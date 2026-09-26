# Keymap tests (luztest)

Behavioural tests for the Luz keymaps: press keys by position, check the USB reports the
host receives. Scenarios are **runner-agnostic**: they don't know about QMK, matrices or
pins. A runner (adapter) replays them on some implementation of the firmware and hands
back what it observed; the CLI does the judging.

```sh
uv run tests/luztest.py run                           # every target, every scenario
uv run tests/luztest.py run -t luz_for_qwerty@kaly/kaly42 -k compose -v
uv run tests/luztest.py list                          # targets and scenario names
uv run tests/luztest.py check                         # validate scenario files only
```

`uv` fetches the one dependency (PyYAML). Exit status: `0` all passed, `1` a scenario
failed, `2` a configuration, build or harness error. `--junit FILE` writes a JUnit report.

## Layout

| Path | What |
|---|---|
| `luztest.yaml` | Targets (keymap × keyboard → scenario file) and per-runner settings |
| `scenarios/<keymap>.yaml` | The scenarios for one keymap |
| `luztest/scenario.py` | Scenario format, validation, and the step timeline (shared by every runner) |
| `luztest/compare.py` | The verdict: observed reports vs `expect` |
| `luztest/adapters/qmk.py` | QMK runner (QMK's host unit-test harness) |

## Scenario format

```yaml
keymap: luz_for_qwerty
defaults: {os: linux, tap: 10}
scenarios:
  - name: mod-tap held past TAPPING_TERM becomes its modifier
    os: linux                     # optional; linux | macos (Luz's OS setting)
    steps: [{down: 26}, {wait: 300}, {up: 26}]
    expect:
      - {report: [LALT], at: 240}
      - []
```

### Steps

Keys are Luz positions **0–41** (see `CLAUDE.md`). Time starts at 0 and each press or
release takes one scan (1 ms), so the timeline is exact and identical on every runner.

| Step | Meaning |
|---|---|
| `{down: N}` / `{down: [N, M]}` | press (in order, 1 ms apart) |
| `{up: N}` | release |
| `{tap: N}` | press, hold `tap` ms (default 10), release; `{tap: N, hold: 300}` overrides |
| `{wait: MS}` | let time pass |

Every key pressed must be released by the end. After the last step the runner keeps
recording for `settle_ms` so timer-driven reports (tap-hold, Caps Word...) are captured.

### Expect

The **ordered list of reports the host receives**, nothing more, nothing less. A keyboard
report is a snapshot of what is held: `[LSFT, A]` (compared as a set), `[]` for "all
released". Names are QMK's without `KC_` (`A`, `QUOT`, `LSFT`, `RALT`...). Keys are
*physical* keys: `[RALT, QUOT]` is a dead acute only under the host's layout, which keeps
tests independent of the host.

| Entry | Meaning |
|---|---|
| `[LSFT, A]` | keyboard report |
| `{report: [LALT], at: 240}` | ...observed at 240 ms, within the runner's `tolerance_ms`, never before |
| `{consumer: MUTE}` / `{consumer: none}` | consumer-page report (media keys), and its release |

`expect: []` means *no report at all*.

## Runners

### qmk

Builds the variant's real `keymap.c`, `config.h` and `rules.mk` (so `luz/luz.h` and every
shared feature) against QMK core for the host, inside QMK's GoogleTest harness. The matrix
and the clock are simulated: positions map to the keyboard's own matrix through its
`LAYOUT` in `qmk info`, and time advances 1 ms per scan, so timings are exact
(`tolerance_ms: 0`). Each scenario runs in its own process, so no state leaks between them.

It needs a `qmk_firmware` checkout (`qmk_home` in `luztest.yaml`, else `$QMK_HOME`, else
`qmk config user.qmk_home`) with the `lib/googletest` and `lib/printf` submodules, the
`qmk` CLI, and a host C/C++ toolchain. It writes a throwaway suite to
`<qmk_home>/tests/luztest_<target>/` (the only place QMK discovers tests) and removes it
afterwards; `--keep` leaves it for debugging.

What it does **not** cover: the split link, USB descriptors and the board definition (the
firmware build in CI covers those), and NKRO (QMK's harness can't build it; Luz boards
send 6-key reports unless NKRO is toggled on).

**QMK version matters.** Expectations describe the QMK that CI builds the firmware with
(`master`). A local `qmk_firmware` at another version can legitimately produce different
reports: for example, key overrides that add Shift gained a 10 ms delay in 2026.

## Adding a runner

Implement the adapter contract in `luztest/adapters/__init__.py` (`run(scenarios)` returns
observed reports per scenario, plus a `tolerance`), register it in `ADAPTERS`, and list it
under `runners:` in `luztest.yaml`. Replay `Scenario.actions`, the precomputed timeline,
rather than re-reading the YAML, so every runner means the same thing by a step. An
emulator runner would turn position N into a GPIO level on the right MCU (both boards use
direct pins) and capture HID reports; its `tolerance_ms` should be measured, not guessed.
