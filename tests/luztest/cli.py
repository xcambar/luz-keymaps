"""luztest — replay Luz scenarios on a runner and check the reports.

    luztest run  [-t KEYMAP@KEYBOARD]... [-r RUNNER] [-k TEXT] [-v] [--keep] [--junit FILE]
    luztest list [-t KEYMAP@KEYBOARD]...
    luztest check                       # validate scenario files only (no runner)

Targets (keymap x keyboard) and per-runner settings live in tests/luztest.yaml.
Exit status: 0 all passed, 1 a scenario failed, 2 usage/config/build error.
"""
from __future__ import annotations

import argparse
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from xml.sax.saxutils import escape

import yaml

from .adapters import ADAPTERS
from .adapters.qmk import AdapterError
from .compare import Verdict, compare
from .scenario import ScenarioError, Suite, load

TESTS_DIR = Path(__file__).resolve().parent.parent
USERSPACE = TESTS_DIR.parent
DEFAULT_CONFIG = TESTS_DIR / "luztest.yaml"


@dataclass
class Target:
    keymap: str
    keyboard: str
    scenarios: Path
    runners: list[str]

    @property
    def id(self) -> str:
        return f"{self.keymap}@{self.keyboard}"


def _color(enabled: bool):
    def c(code, s):
        return f"\033[{code}m{s}\033[0m" if enabled else s
    return c


def load_config(path: Path) -> tuple[list[Target], dict]:
    data = yaml.safe_load(path.read_text(encoding="utf-8")) or {}
    runners = data.get("runners") or {}
    targets = []
    for i, t in enumerate(data.get("targets") or []):
        try:
            scen = (path.parent / t["scenarios"]).resolve()
            targets.append(Target(t["keymap"], t["keyboard"], scen, t.get("runners") or list(runners) or ["qmk"]))
        except KeyError as e:
            raise ScenarioError(f"{path.name}: target {i + 1} is missing {e}") from None
    return targets, runners


def select(targets: list[Target], wanted: list[str]) -> list[Target]:
    if not wanted:
        return targets
    out = []
    for w in wanted:
        km, _, kb = w.partition("@")
        hits = [t for t in targets if t.keymap == km and (not kb or t.keyboard == kb)]
        if not hits:
            raise ScenarioError(f"no target matches {w!r} (known: {', '.join(t.id for t in targets)})")
        out += [h for h in hits if h not in out]
    return out


def cmd_check(targets: list[Target], out) -> int:
    seen = set()
    for t in targets:
        if t.scenarios in seen:
            continue
        seen.add(t.scenarios)
        suite = load(t.scenarios)
        if suite.keymap != t.keymap:
            raise ScenarioError(f"{t.scenarios.name} is for {suite.keymap}, but target {t.id} uses it")
        print(f"ok  {t.scenarios.relative_to(USERSPACE)}: {len(suite.scenarios)} scenarios", file=out)
    return 0


def cmd_list(targets: list[Target], out) -> int:
    for t in targets:
        suite = load(t.scenarios)
        print(f"{t.id}  [{', '.join(t.runners)}]", file=out)
        for s in suite.scenarios:
            print(f"  {s.index:3d}  {s.name}  ({s.os})", file=out)
    return 0


def junit(results: list[tuple[str, str, Verdict | str]], path: Path) -> None:
    by_suite: dict[str, list] = {}
    for suite, runner, v in results:
        by_suite.setdefault(f"{suite} [{runner}]", []).append(v)
    xml = ['<?xml version="1.0" encoding="UTF-8"?>', "<testsuites>"]
    for name, vs in by_suite.items():
        fails = sum(1 for v in vs if isinstance(v, str) or not v.passed)
        xml.append(f'  <testsuite name="{escape(name)}" tests="{len(vs)}" failures="{fails}">')
        for v in vs:
            if isinstance(v, str):
                xml.append(f'    <testcase name="(setup)"><error message="{escape(v, {chr(34): "&quot;"})}"/></testcase>')
                continue
            xml.append(f'    <testcase name="{escape(v.scenario.name, {chr(34): "&quot;"})}">')
            if not v.passed:
                msg = "; ".join(v.problems)
                xml.append(f'      <failure message="{escape(msg, {chr(34): "&quot;"})}"/>')
            xml.append("    </testcase>")
        xml.append("  </testsuite>")
    xml.append("</testsuites>")
    path.write_text("\n".join(xml) + "\n", encoding="utf-8")


def cmd_run(args, targets: list[Target], runner_cfg: dict, out) -> int:
    c = _color(out.isatty() and not args.no_color)
    results: list[tuple[str, str, Verdict | str]] = []
    failed = errored = 0
    for t in targets:
        suite: Suite = load(t.scenarios)
        if suite.keymap != t.keymap:
            raise ScenarioError(f"{t.scenarios.name} is for {suite.keymap}, but target {t.id} uses it")
        scenarios = [s for s in suite.scenarios if not args.filter or args.filter.lower() in s.name.lower()]
        for rname in ([args.runner] if args.runner else t.runners):
            if rname not in ADAPTERS:
                raise ScenarioError(f"unknown runner {rname!r} (available: {', '.join(ADAPTERS)})")
            settings = dict(runner_cfg.get(rname) or {})
            if args.keep:
                settings["keep"] = True
            print(c("1", f"== {t.id} on {rname} =="), file=out)
            if not scenarios:
                print("   no scenario matches the filter", file=out)
                continue
            started = time.monotonic()
            log = (lambda m: print(c("2", f"   {m}"), file=out)) if args.verbose else (lambda m: None)
            try:
                adapter = ADAPTERS[rname](t, settings, USERSPACE, log)
                observed = adapter.run(scenarios)
            except AdapterError as e:
                print(c("31", f"   ERROR {e}"), file=out)
                results.append((t.id, rname, str(e)))
                errored += 1
                continue
            for s in scenarios:
                obs = observed.get(s.index)
                if isinstance(obs, str):
                    print(c("31", f"   ERROR {s.name}\n") + _indent(obs, 9), file=out)
                    results.append((t.id, rname, obs))
                    errored += 1
                    continue
                v = compare(s, obs, adapter.tolerance)
                results.append((t.id, rname, v))
                if v.passed:
                    print(c("32", "   pass ") + s.name, file=out)
                    if args.verbose:
                        print(_indent("  ".join(o.describe() for o in obs), 9), file=out)
                    continue
                failed += 1
                print(c("31", "   FAIL ") + s.name, file=out)
                for p in v.problems:
                    print(f"         {p}", file=out)
                print(f"         expected: {'  '.join(e.describe() for e in s.expect) or '(no reports)'}", file=out)
                print(f"         observed: {'  '.join(o.describe() for o in obs) or '(no reports)'}", file=out)
            print(c("2", f"   {time.monotonic() - started:.1f}s"), file=out)
    total = sum(1 for r in results if isinstance(r[2], Verdict))
    summary = f"{total - failed} passed, {failed} failed" + (f", {errored} errors" if errored else "")
    print(c("1;32" if not failed and not errored else "1;31", summary), file=out)
    if args.junit:
        junit(results, Path(args.junit))
    return 2 if errored else 1 if failed else 0


def _indent(text: str, n: int) -> str:
    return "\n".join(" " * n + line for line in text.splitlines())


def main(argv=None) -> int:
    p = argparse.ArgumentParser(prog="luztest", description=__doc__.split("\n\n")[0])
    p.add_argument("-c", "--config", type=Path, default=DEFAULT_CONFIG, help="targets/runners file (default: tests/luztest.yaml)")
    sub = p.add_subparsers(dest="cmd", required=True)
    for name in ("run", "list", "check"):
        sp = sub.add_parser(name)
        sp.add_argument("-t", "--target", action="append", default=[], metavar="KEYMAP[@KEYBOARD]",
                        help="restrict to these targets (repeatable); default: all")
        if name == "run":
            sp.add_argument("-r", "--runner", help="override the target's runners (e.g. qmk)")
            sp.add_argument("-k", "--filter", help="only scenarios whose name contains TEXT")
            sp.add_argument("-v", "--verbose", action="store_true", help="show steps and every observed trace")
            sp.add_argument("--keep", action="store_true", help="keep the generated harness for debugging")
            sp.add_argument("--junit", metavar="FILE", help="also write a JUnit XML report")
            sp.add_argument("--no-color", action="store_true")
    args = p.parse_args(argv)
    out = sys.stdout
    try:
        targets, runner_cfg = load_config(args.config)
        targets = select(targets, args.target)
        if args.cmd == "check":
            return cmd_check(targets, out)
        if args.cmd == "list":
            return cmd_list(targets, out)
        return cmd_run(args, targets, runner_cfg, out)
    except (ScenarioError, FileNotFoundError, yaml.YAMLError) as e:
        print(f"luztest: {e}", file=sys.stderr)
        return 2
