# keyspec

**Behavioural tests for keyboard firmware.** Press keys by position on a precise
timeline, and check the USB reports the host receives, including when it receives them.

```yaml
- name: mod-tap held past TAPPING_TERM becomes its modifier
  steps: [{down: 26}, {wait: 300}, {up: 26}]
  expect:
    - {report: [LALT], at: 240}
    - []
```

Scenarios describe **observable behaviour only**: key positions in, HID reports out. They
don't know about keycodes, matrices, pins or the firmware's internals, so the same file
can check a keymap on several keyboards, and a keymap ported to another firmware against
the same expectations. Running them is the job of a **runner adapter**. The first one,
`qmk`, compiles your real QMK keymap into QMK's host test harness, simulates the matrix
and the clock, and records every report.

Combos, layers, tap-hold (permissive hold, Chordal Hold, Flow Tap), Caps Word, key
overrides, macros, custom `process_record_user` logic, community modules: if it changes
what the host receives, a scenario can pin it down.

## Quick start

Requirements:
- Python ≥ 3.10 and [uv](https://docs.astral.sh/uv/), or PyYAML.
- For the `qmk` runner: a `qmk_firmware` checkout, the `qmk` CLI, and a host C/C++
  toolchain (`gcc`/`g++`, `make`).

In your QMK userspace, create `keyspec.yaml`:

```yaml
runners:
  qmk:
    layout: LAYOUT_split_3x6_3     # whose key order defines the positions
targets:
  - keymap: my_keymap
    keyboard: crkbd/rev1
    scenarios: tests/my_keymap.yaml
```

...and `tests/my_keymap.yaml`:

```yaml
scenarios:
  - name: the key at position 13 types a
    steps: [{tap: 13}]
    expect: [[A], []]
```

Then (keyspec currently lives inside the Luz repository; it installs straight from there):

```sh
uvx --from 'git+https://github.com/xcambar/luz-keymaps#subdirectory=packages/keyspec' keyspec run
# or, from a checkout:   uv run --project path/to/keyspec keyspec run
```

| Command | What |
|---|---|
| `keyspec run` | build and run every target; `-t KEYMAP[@KEYBOARD]`, `-k TEXT` (name filter), `-v`, `--junit FILE`, `--keep` |
| `keyspec list` | targets and their scenarios |
| `keyspec check` | validate scenario files without building anything |

Exit status: `0` all passed, `1` a scenario failed, `2` a configuration, build or harness
error. `keyspec.yaml` is found in the current directory or above, or given with `-c`.

## Scenarios

```yaml
keymap: my_keymap          # optional: refuse to run these against another keymap
defaults:
  tap: 10                  # ms a `tap` holds its key (default 10)
  setup: linux             # fixtures applied before step 1 (see "Setups")
scenarios:
  - name: unique name
    description: optional prose
    setup: [macos]         # overrides defaults.setup
    steps: [...]
    expect: [...]
```

### Positions

A position is an index into the keyboard's layout: `0` is the first key the `LAYOUT`
macro lists, `1` the next, and so on. That is the order you write `keymaps[]` in, so
position N is the Nth keycode of each layer. Keyboards that share a layout share
positions, even when their matrices differ.

### Steps

Time starts at 0. Each press or release takes one scan (1 ms), so the timeline is exact
and identical for every runner.

| Step | Meaning |
|---|---|
| `{down: N}` / `{down: [N, M]}` | press (in order, 1 ms apart) |
| `{up: N}` | release |
| `{tap: N}` | press, hold `tap` ms, release; `{tap: N, hold: 300}` overrides the hold |
| `{wait: MS}` | let time pass |

Every key pressed must be released by the end. After the last step the runner keeps
recording for `settle_ms`, so timer-driven reports (tap-hold, combos, Caps Word...) are
captured.

### Expect

The **ordered list of reports the host receives**, nothing more and nothing less. A
keyboard report is a snapshot of what is held: `[LSFT, A]` (compared as a set), and `[]`
for "all released". Names are QMK's without `KC_` (`A`, `QUOT`, `LSFT`, `RALT`...).
Keys are *physical*: `[RALT, QUOT]` is whatever the host's layout makes of it, which keeps
tests independent of the host.

| Entry | Meaning |
|---|---|
| `[LSFT, A]` | a keyboard report |
| `{report: [LALT], at: 240}` | ...received at 240 ms (within the runner's tolerance, never before) |
| `{consumer: MUTE}` / `{consumer: none}` | a consumer-page report (media keys), and its release |

`expect: []` means *no report at all*. When a scenario fails, keyspec prints both
sequences with timestamps. Writing the expectation from what you believe the firmware
does, then reading the diff, is usually how you find out what it really does.

### Setups

Some behaviour depends on state that isn't a key press: which OS the firmware thinks it
talks to, a toggled mode, a value in EEPROM. A scenario names the fixtures it needs
(`setup: macos`), and each runner defines them. For `qmk` they are C statements run after
the keymap is loaded and before step 1:

```yaml
runners:
  qmk:
    setups:
      macos:
        includes: [my/os_state.h]
        code: "set_host_os(OS_MACOS);"
```

## Runners

### qmk

It builds the keymap's `keymap.c`, `config.h` and `rules.mk` (and so everything they
include: userspace code, `keymap.json` community modules) against QMK core for the host,
inside QMK's GoogleTest harness.

- **Simulated:** the matrix and the clock. Positions map to the keyboard's own matrix
  through its `LAYOUT` in `qmk info`, and time advances 1 ms per scan, so timings are
  exact.
- **Isolated:** each scenario runs in its own process, so no state leaks between them.
- **Throwaway suite:** it writes the generated suite to `<qmk_home>/tests/keyspec_<target>/`
  (the only place QMK discovers tests) and removes it afterwards. `--keep` leaves it for
  debugging.

| Setting (`runners.qmk`, or per target under `settings.qmk`) | Default |
|---|---|
| `qmk_home` | `$QMK_HOME`, then `qmk config user.qmk_home` |
| `layout` | the keyboard's only layout |
| `tolerance_ms` | `0` |
| `settle_ms` | `1000` |
| `undefine` | split-only `config.h` macros (`SPLIT_WATCHDOG_ENABLE`...), `#undef`'d for the host build |
| `setups` | none |
| `jobs` | CPU count |

The `qmk_firmware` checkout needs the `lib/googletest` and `lib/printf` submodules.

**What it does not cover:**
- the split transport, USB descriptors and the board definition (a normal firmware build
  covers those);
- NKRO (QMK's harness can't build it; keyboards send 6-key reports unless NKRO is toggled
  on);
- anything keyed to real hardware.

**Pin the QMK version.** Expectations describe a specific QMK. Upstream changes can
legitimately change the report stream: for example, in 2026 key overrides that add a
modifier started sending it 10 ms before the key. Run the tests against the same QMK you
build the firmware with.

### Writing another runner

An adapter replays `Scenario.actions`, a precomputed timeline of `(t, down/up, position)`,
and returns the reports it observed with timestamps. It never re-reads the YAML, so every
runner means the same thing by a step. It also declares a `tolerance` in ms: the latency
it can't avoid, which should be measured rather than guessed. The contract is in
`src/keyspec/adapters/__init__.py`.

Candidates: ZMK's native POSIX build, an RP2040/STM32 emulator driving GPIOs (with direct
pins, position N is a single pin), or hardware in the loop.

## CI

```yaml
- run: |
    git clone --depth 1 https://github.com/qmk/qmk_firmware.git qmk_firmware
    git -C qmk_firmware submodule update --init --depth 1 lib/googletest lib/printf
- uses: astral-sh/setup-uv@v6
- run: uv tool install qmk
- run: uvx --from 'git+https://github.com/xcambar/luz-keymaps#subdirectory=packages/keyspec' keyspec run --junit keyspec.xml
  env:
    QMK_HOME: ${{ github.workspace }}/qmk_firmware
```

## License

GPL-2.0-or-later, like QMK, whose test harness the `qmk` runner builds against.
