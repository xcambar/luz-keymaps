"""QMK adapter: replays scenarios inside QMK's host unit-test harness (tests/ + GoogleTest).

What runs is the real keymap: the variant's keymap.c (and everything it includes, i.e.
luz/luz.h) plus its config.h and rules.mk, compiled against QMK core for the host. What
is simulated is the matrix (keys are pressed by position, via the keyboard's own
LAYOUT -> matrix mapping from `qmk info`) and the clock (1 ms per scan, deterministic).

Per target it:
  1. reads the keyboard's layout and features with `qmk info`;
  2. generates a throwaway test suite in <qmk_home>/tests/luztest_<target>/ — QMK only
     discovers tests there — with one gtest case per scenario;
  3. builds it with QMK's build_test.mk, without running it;
  4. runs each case in its own process (no state leaks between scenarios) and parses
     the timestamped reports each one prints.
"""
from __future__ import annotations

import json
import os
import re
import shutil
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from ..compare import Observed
from ..scenario import CONSUMER_NAMES, MODS, POSITIONS, Scenario

# Keyboard features that change what the host receives, so the harness must match the
# real firmware on them. Everything else in keyboard.json is hardware and stays out.
# NKRO is left out: QMK's harness can't build it, and without NKRO_DEFAULT_ON the real
# firmware sends the same 6-key reports until NKRO is toggled on.
REPORT_FEATURES = ("extrakey", "mousekey")
REPORT_ID_CONSUMER = 4

# Keymap config.h settings that only make sense on the split hardware (the harness is a
# single, unsplit board); undefined after the keymap's config.h is included.
HARDWARE_ONLY = ("SPLIT_WATCHDOG_ENABLE", "SPLIT_WATCHDOG_TIMEOUT", "SPLIT_USB_DETECT", "SPLIT_USB_TIMEOUT")

TRACE = re.compile(r"^LUZTRACE (K|N|C) (\d+) ?(.*)$")


class AdapterError(RuntimeError):
    pass


def _run(cmd, **kw) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, capture_output=True, text=True, **kw)


class QmkAdapter:
    name = "qmk"

    def __init__(self, target, settings: dict, userspace: Path, log):
        self.target = target
        self.userspace = userspace
        self.log = log
        self.tolerance = int(settings.get("tolerance_ms", 0))
        self.settle = int(settings.get("settle_ms", 1000))
        self.jobs = int(settings.get("jobs", os.cpu_count() or 2))
        self.keep = bool(settings.get("keep", False))
        self.layout = settings.get("layout", "LAYOUT_split_3x6_3")
        self.undefine = list(settings.get("undefine", HARDWARE_ONLY))
        self.qmk_home = self._qmk_home(settings.get("qmk_home"))
        slug = re.sub(r"[^A-Za-z0-9]+", "_", f"{target.keymap}_{target.keyboard}").strip("_")
        self.test_name = f"luztest_{slug}"
        self.test_dir = self.qmk_home / "tests" / self.test_name
        self.env = dict(os.environ, QMK_USERSPACE=str(userspace))

    # ── setup ────────────────────────────────────────────────────────────────

    def _qmk_home(self, configured) -> Path:
        home = configured or os.environ.get("QMK_HOME")
        if not home and shutil.which("qmk"):
            out = _run(["qmk", "config", "-ro", "user.qmk_home"]).stdout.strip()
            home = out.split("=", 1)[1] if "=" in out else None
            if home == "None":
                home = None
        if not home:
            raise AdapterError("cannot find qmk_firmware: set qmk_home, QMK_HOME, or `qmk config user.qmk_home`")
        path = Path(home).expanduser().resolve()
        if not (path / "builddefs" / "build_test.mk").is_file():
            raise AdapterError(f"{path} does not look like a qmk_firmware checkout")
        return path

    def _keyboard_info(self) -> dict:
        res = _run(["qmk", "info", "-kb", self.target.keyboard, "-f", "json"], cwd=self.userspace, env=self.env)
        if res.returncode != 0:
            raise AdapterError(f"`qmk info -kb {self.target.keyboard}` failed:\n{res.stderr or res.stdout}")
        return json.loads(res.stdout)

    def _keymap_dir(self) -> Path:
        for base in (self.userspace, self.qmk_home):
            d = base / "keyboards" / self.target.keyboard
            for parent in [d, *d.parents]:
                cand = parent / "keymaps" / self.target.keymap
                if cand.is_dir():
                    return cand.resolve()
                if parent == base / "keyboards":
                    break
        raise AdapterError(f"keymap {self.target.keymap!r} not found for {self.target.keyboard}")

    # ── generation ───────────────────────────────────────────────────────────

    def _generate(self, scenarios: list[Scenario]) -> None:
        info = self._keyboard_info()
        layout = info.get("layouts", {}).get(self.layout, {}).get("layout")
        if not layout:
            raise AdapterError(f"{self.target.keyboard} has no {self.layout}")
        if len(layout) != POSITIONS:
            raise AdapterError(f"{self.layout} has {len(layout)} keys, scenarios address {POSITIONS}")
        matrix = [tuple(k["matrix"]) for k in layout]
        rows = max(r for r, _ in matrix) + 1
        cols = max(c for _, c in matrix) + 1
        keymap_dir = self._keymap_dir()
        features = info.get("features", {})

        if self.test_dir.exists():
            shutil.rmtree(self.test_dir)
        self.test_dir.mkdir(parents=True)

        # LAYOUT macro: the keyboard's own position -> matrix mapping, KC_NO elsewhere.
        args = [f"k{i:02d}" for i in range(POSITIONS)]
        grid = [["KC_NO"] * cols for _ in range(rows)]
        for i, (r, c) in enumerate(matrix):
            grid[r][c] = args[i]
        body = ", ".join("{" + ", ".join(row) + "}" for row in grid)
        config = [
            "// Generated by tests/luztest (QMK adapter). Do not edit.",
            "#pragma once",
            f"#define MATRIX_ROWS {rows}",
            f"#define MATRIX_COLS {cols}",
            '#define QMK_KEYBOARD_H "quantum.h"',
            f"#define {self.layout}({', '.join(args)}) {{ {body} }}",
        ]
        if (keymap_dir / "config.h").is_file():
            config.append(f'#include "{keymap_dir / "config.h"}"')
        config += [f"#undef {m}" for m in self.undefine]
        (self.test_dir / "config.h").write_text("\n".join(config) + "\n")

        mk = ["# Generated by tests/luztest (QMK adapter). Do not edit."]
        mk += [f"{f.upper()}_ENABLE = yes" for f in REPORT_FEATURES if features.get(f)]
        if (keymap_dir / "rules.mk").is_file():
            mk.append(f"include {keymap_dir / 'rules.mk'}")
        # Keymap rules add their header dirs to VPATH (for firmware builds, VPATH is on the
        # include path). The test build doesn't do that, so pass them as -I explicitly.
        mk.append("LUZTEST_INC := $(addprefix -I,$(VPATH))")
        mk.append("EXTRAFLAGS += $(LUZTEST_INC)")
        (self.test_dir / "test.mk").write_text("\n".join(mk) + "\n")

        # keymap_introspection.c includes KEYMAP_C ("keymap.c"): the test dir comes first
        # on the search path, so this pulls the variant's real keymap in.
        (self.test_dir / "keymap.c").write_text(
            "// Generated by tests/luztest (QMK adapter). Do not edit.\n"
            f'#include "{keymap_dir / "keymap.c"}"\n')

        (self.test_dir / "test_luztest.cpp").write_text(self._cpp(scenarios, matrix))

    def _cpp(self, scenarios: list[Scenario], matrix) -> str:
        pos = ", ".join(f"{{{r}, {c}}}" for r, c in matrix)
        cases = []
        for s in scenarios:
            acts = ", ".join(f"{{{a.t}, {'true' if a.kind == 'down' else 'false'}, {a.pos}}}" for a in s.actions)
            os_ = "OS_MacOS" if s.os == "macos" else "OS_Linux"
            cases.append(
                f"// {s.name}\n"
                f"TEST_F(Luz, S{s.index}) {{\n"
                f"    static const Act acts[] = {{{acts or '{0, false, 255}'}}};\n"
                f"    play({os_}, acts, {len(s.actions)}, {s.duration});\n"
                f"}}\n")
        return CPP_TEMPLATE.replace("@POS@", pos).replace("@SETTLE@", str(self.settle)) \
            .replace("@CASES@", "\n".join(cases))

    # ── build & run ──────────────────────────────────────────────────────────

    def _build(self) -> Path:
        name = self.test_name
        cmd = ["make", "-r", "-R", "-s", "-C", str(self.qmk_home), "-f", "builddefs/build_test.mk",
               f"-j{self.jobs}", f"TEST={name}", f"TEST_OUTPUT={name}", f"TEST_PATH=tests/{name}",
               f"FULL_TESTS={name}", f"QMK_USERSPACE={self.userspace}", "SKIP_GIT=yes", "SILENT=true"]
        res = _run(cmd, env=self.env)
        elf = self.qmk_home / ".build" / "test" / f"{name}.elf"
        if res.returncode != 0 or not elf.is_file():
            out = (res.stdout + res.stderr).strip().splitlines()
            errors = [l for l in out if "error" in l.lower()] or out[-40:]
            raise AdapterError("test build failed:\n" + "\n".join(errors[:40]))
        return elf

    def _run_case(self, elf: Path, s: Scenario) -> list[Observed] | str:
        res = _run([str(elf), f"--gtest_filter=Luz.S{s.index}"], cwd=self.qmk_home)
        observed = []
        for line in res.stdout.splitlines():
            m = TRACE.match(line)
            if m:
                observed.append(self._parse(m))
        if res.returncode != 0:
            tail = "\n".join((res.stdout + res.stderr).strip().splitlines()[-25:])
            return f"harness exited with {res.returncode}:\n{tail}"
        return observed

    @staticmethod
    def _parse(m) -> Observed:
        kind, t, rest = m.group(1), int(m.group(2)), m.group(3).split()
        if kind == "C":
            report_id, usage = int(rest[0]), int(rest[1])
            if report_id != REPORT_ID_CONSUMER:
                return Observed(t, f"extra:{report_id}", str(usage))
            return Observed(t, "consumer", "none" if usage == 0 else CONSUMER_NAMES.get(usage, f"0x{usage:04X}"))
        mods = int(rest[0], 16)
        names = {MODS[b] for b in range(8) if mods & (1 << b)}
        names |= {k[3:] if k.startswith("KC_") else k for k in rest[1:]}
        return Observed(t, "keyboard", frozenset(names))

    def run(self, scenarios: list[Scenario]) -> dict[int, list[Observed] | str]:
        try:
            self.log(f"generating {self.test_dir.relative_to(self.qmk_home)}")
            self._generate(scenarios)
            self.log("building (QMK test harness)")
            elf = self._build()
            self.log(f"running {len(scenarios)} scenarios")
            with ThreadPoolExecutor(self.jobs) as pool:
                results = list(pool.map(lambda s: self._run_case(elf, s), scenarios))
            return {s.index: r for s, r in zip(scenarios, results)}
        finally:
            if not self.keep and self.test_dir.exists():
                shutil.rmtree(self.test_dir)


CPP_TEMPLATE = r"""// Generated by tests/luztest (QMK adapter). Do not edit.
// Each case replays one scenario's timeline and prints every report the host would get.
#include <cstdio>
#include "keyboard_report_util.hpp"
#include "test_common.hpp"

extern "C" {
#include "keymap_introspection.h"
#include "luz/os_control.h"
}

using testing::_;
using testing::AnyNumber;
using testing::Invoke;

namespace {
struct Pos { uint8_t row, col; };
struct Act { uint32_t t; bool down; uint8_t pos; };
const Pos POS[] = {@POS@};
const uint32_t SETTLE_MS = @SETTLE@;
}

class Luz : public TestFixture {
   protected:
    void play(uint8_t os, const Act* acts, size_t n, uint32_t duration) {
        // The real keymap, every layer, as compiled from the variant's keymap.c.
        for (uint8_t layer = 0; layer < keymap_layer_count(); layer++)
            for (uint8_t row = 0; row < MATRIX_ROWS; row++)
                for (uint8_t col = 0; col < MATRIX_COLS; col++)
                    add_key(KeymapKey(layer, col, row, keycode_at_keymap_location_raw(layer, row, col)));
        while (get_os_platform() != os) toggle_os_platform();

        TestDriver driver;
        const uint32_t t0 = timer_read32();
        auto now = [&] { return (unsigned)(timer_read32() - t0); };
        EXPECT_CALL(driver, send_keyboard_mock(_)).WillRepeatedly(Invoke([&](report_keyboard_t& r) {
            printf("LUZTRACE K %u %02x", now(), r.mods);
            for (int i = 0; i < KEYBOARD_REPORT_KEYS; i++)
                if (r.keys[i]) printf(" %s", get_keycode_string(r.keys[i]));
            printf("\n");
        }));
        EXPECT_CALL(driver, send_nkro_mock(_)).WillRepeatedly(Invoke([&](report_nkro_t& r) {
            printf("LUZTRACE K %u %02x", now(), r.mods);
            for (int k = 0; k < NKRO_REPORT_BITS * 8; k++)
                if (r.bits[k / 8] & (1 << (k % 8))) printf(" %s", get_keycode_string(k));
            printf("\n");
        }));
        EXPECT_CALL(driver, send_extra_mock(_)).WillRepeatedly(Invoke([&](report_extra_t& r) {
            printf("LUZTRACE C %u %u %u\n", now(), r.report_id, r.usage);
        }));
        EXPECT_CALL(driver, send_mouse_mock(_)).Times(AnyNumber());

        for (size_t i = 0; i < n; i++) {
            if (acts[i].pos >= sizeof(POS) / sizeof(POS[0])) continue;
            if (now() < acts[i].t) idle_for(acts[i].t - now());
            const Pos p = POS[acts[i].pos];
            if (acts[i].down) press_key(p.col, p.row); else release_key(p.col, p.row);
            run_one_scan_loop();
        }
        // Let timers (tap-hold, combos, Caps Word...) play out, still recording.
        if (now() < duration + SETTLE_MS) idle_for(duration + SETTLE_MS - now());
        fflush(stdout);
        testing::Mock::VerifyAndClearExpectations(&driver);
    }
};

@CASES@
"""
