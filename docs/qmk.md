# The QMK reference implementation

This repository is a [QMK external userspace](https://docs.qmk.fm/newbs_external_userspace)
holding four reference implementations of [the Luz specification](../spec/README.md), one per
alpha layout. They are *reference* implementations: when they and the spec disagree, the spec
wins. They are also the way to try Luz today, on any 42-key split keyboard QMK supports.

| Variant | Letters | Keymap |
|---|---|---|
| Luz for Gallium | Gallium East | [`luz_for_gallium`](../keyboards/6x3_3/keymaps/luz_for_gallium/) |
| Luz for Enthium | Enthium | [`luz_for_enthium`](../keyboards/6x3_3/keymaps/luz_for_enthium/) |
| Luz for Colemak-DH | Colemak Mod-DH (matrix) | [`luz_for_colemak_dh`](../keyboards/6x3_3/keymaps/luz_for_colemak_dh/) |
| Luz for QWERTY | QWERTY | [`luz_for_qwerty`](../keyboards/6x3_3/keymaps/luz_for_qwerty/) |

## Layout of the userspace

```
modules/luz/                  QMK community modules: the OS-aware features, reusable on their own
keyboards/6x3_3/
  luz/                        what every variant shares
    luz.h                     the shared keymap.c body (combos, key overrides, macros, Caps Word)
    layers.h symbols.h mods.h compose_combo.h custom_keycodes.h
    config.h rules.mk         tap-hold tuning and features; variants include these
    keymap_drawer/            diagram build (build_pdf.sh <keymap>)
  keymaps/luz_for_*/          the variants
    keymap.c                  keymaps[] and the position-bound defines, then #include "luz/luz.h"
    keymap.json               the community modules, in processing order
    layouts/*.h               BASE letters as position macros (_00_ … _41_)
    config.h rules.mk         one-line includes of luz/
keyboards/kaly/kaly42/…       symlinks to the variants (STM32)
keyboards/42keebs/…           Cantor Pro v3 (RP2040), symlinking the whole keymaps dir
tests/                        conformance scenarios and keyspec targets
packages/keyspec/             the test runner
```

## From the spec to the code

| Spec | Where it lives |
|---|---|
| [Frame](../spec/frame.md), [layers](../spec/layers.md) | each variant's `keymaps[]`; `luz/layers.h`; the tri-layer in `luz/luz.h` |
| [Modifiers](../spec/mods.md) | mod-taps in `keymaps[]`; Chordal Hold map in `luz/mods.h`; tuning in `luz/config.h`; [`luz/cmd_ctrl_morph`](../modules/luz/cmd_ctrl_morph/), [`luz/mod_latch`](../modules/luz/mod_latch/) |
| [Symbols](../spec/symbols.md) | `SYMBOL_TABLE` in `luz/symbols.h` (keycodes and key overrides), `SYM_MODTAP_SHIFT` |
| [Compose](../spec/compose.md) | the combo in `luz/compose_combo.h`; [`luz/compose`](../modules/luz/compose/), [`luz/dead_keys`](../modules/luz/dead_keys/), [`luz/semantic_keys`](../modules/luz/semantic_keys/) |
| [Navigation](../spec/navigation.md) | `keymaps[EXTEND…]`; [`luz/semantic_keys`](../modules/luz/semantic_keys/), [`luz/swapper`](../modules/luz/swapper/) |
| host OS setting | [`luz/host_os`](../modules/luz/host_os/) |

### Module order

Every variant's `keymap.json` lists the modules in the order they must see each key:

```
compose → dead_keys → semantic_keys → swapper → cmd_ctrl_morph → mod_latch → host_os
```

QMK hands every key to the modules in that order, before `process_record_user`, and a
module that consumes a key hides it from the ones after it:
- **Compose first:** it consumes the key after it is armed.
- **The morph before the latch:** a morphed Ctrl can be latched.
- **Everything else** keeps the order the features had when they were one function.

## Building

```sh
qmk config user.overlay_dir="$(pwd)"      # once: point the QMK CLI at this userspace
qmk compile -kb kaly/kaly42 -km luz_for_gallium
qmk userspace-compile                     # all targets in qmk.json
```

If a build fails with `Module 'luz/…' not found`, the CLI isn't seeing this userspace:
check `qmk config user.overlay_dir`. A stale value outranks `$QMK_USERSPACE`.

CI builds every target against QMK `master` on each push and pull request, and publishes
firmware on `v*` tags.

## Testing

```sh
uv run --project packages/keyspec keyspec -c tests/keyspec.yaml run
uv run --project packages/keyspec keyspec -c tests/keyspec.yaml run -t luz_for_enthium -k compose -v
```

- **What runs:** the shared conformance scenarios, plus each variant's own, on both
  boards, in QMK's test harness with the real keymaps and modules
  ([Conformance](../spec/conformance.md)).
- **CI:** the job runs against the same QMK as the firmware build, and tagged releases wait
  for it.
- **Local QMK version:** a local `qmk_firmware` far behind `master` can produce different
  report streams (QMK's key overrides changed timing in 2026). Point `QMK_HOME` at a
  current checkout if a scenario fails locally but not in CI.

## Diagrams

```sh
keyboards/6x3_3/luz/keymap_drawer/build_pdf.sh luz_for_gallium
```

It renders each variant's `keymap_drawer/*.yml` into the SVGs its README uses, plus a colour
PDF and a print PDF. The `build-keymap-pdf` command can first sync the YAMLs from `keymap.c`.

## Adding a variant

1. **Copy a variant** whose letters are closest (`luz_for_colemak_dh` uses position macros,
   `luz_for_enthium` writes BASE inline).
2. **Place your letters on BASE, inside the frame.** Choose your privileged symbols and
   your SYMBOLS right half under the
   [placement principles](../spec/symbols.md#placement).
3. **Set `LUZ_MORPH_L/R` and the `SY_*_MODTAP` defines** to what your BASE puts on 17/18 and
   on any symbol mod-taps.
4. **Add it to `qmk.json`**, symlink it for Kaly42, and add targets to `tests/keyspec.yaml`
   with a `tests/scenarios/luz_for_<name>.yaml` of your own.
5. **Run the tests.** `luz.yaml` must pass unchanged.
