# QMK Userspace

## Project Structure

Two keyboards share the same `split_3x6_3` (6 columns, 3 rows + 3 thumb keys per side) layout.

### Keyboards
- **Kaly42** (`kaly/kaly42`) - STM32-based split keyboard
- **Cantor Pro v3** (`42keebs/cantor_pro/v3/left`) - RP2040-based split keyboard by 42Keebs

### Shared Keymaps
Keymaps live in `keyboards/6x3_3/keymaps/` and are symlinked to both keyboards:

```
keyboards/6x3_3/keymaps/
├── luz_for_gallium/   # Luz for Gallium (the daily driver; dead keys, semantic keys, symbols, etc.)
├── luz_for_enthium/   # Luz for Enthium (same Luz conventions on the Enthium layout)
├── luz_for_colemak_dh/ # Luz for Colemak-DH (matrix); the portability test variant
├── luz_for_qwerty/    # Luz for QWERTY; the framework demo — Luz conventions, familiar alphas
└── zen/               # Minimal QWERTY keymap (3 layers: BASE, CODE, NAV)

# kaly symlinks each keymap individually:
keyboards/kaly/kaly42/keymaps/
├── luz_for_gallium -> ../../../6x3_3/keymaps/luz_for_gallium
├── luz_for_enthium -> ../../../6x3_3/keymaps/luz_for_enthium
├── luz_for_colemak_dh -> ../../../6x3_3/keymaps/luz_for_colemak_dh
├── luz_for_qwerty -> ../../../6x3_3/keymaps/luz_for_qwerty
└── zen -> ../../../6x3_3/keymaps/zen

# cantor symlinks the WHOLE keymaps dir (so it picks up all of them):
keyboards/42keebs/cantor_pro/v3/{left,right}/keymaps -> ../../../../6x3_3/keymaps
```

Everything the variants share lives once in `keyboards/6x3_3/luz/`: the C features
(`*.c`/`*.h`), `config.h` and `rules.mk` (each variant's are one-line includes), `luz.h`
(the shared `keymap.c` body, included after `keymaps[]`), and `keymap_drawer/` (the diagram
build: `keyboards/6x3_3/luz/keymap_drawer/build_pdf.sh <keymap>`). A variant holds only its
`keymap.c` (`keymaps[]` + position-bound defines), `layouts/`, docs and diagram YAMLs.

Behavioural tests live in `tests/` (`uv run tests/luztest.py run`; see `tests/README.md`):
scenarios by key position with the expected USB reports, replayed in QMK's host test harness.

Note: QMK forbids hyphens in keymap names, so the dirs use underscores (`luz_for_gallium`),
while the human-facing name is "Luz for Gallium".

### Build Configuration (`qmk.json`)
```json
{
    "userspace_version": "1.1",
    "build_targets": [
        ["kaly/kaly42", "luz_for_gallium"],
        ["kaly/kaly42", "zen"],
        ["kaly/kaly42", "luz_for_enthium"],
        ["kaly/kaly42", "luz_for_colemak_dh"],
        ["kaly/kaly42", "luz_for_qwerty"],
        ["42keebs/cantor_pro/v3/left", "luz_for_gallium"],
        ["42keebs/cantor_pro/v3/right", "luz_for_gallium"],
        ["42keebs/cantor_pro/v3/left", "zen"],
        ["42keebs/cantor_pro/v3/right", "zen"],
        ["42keebs/cantor_pro/v3/left", "luz_for_enthium"],
        ["42keebs/cantor_pro/v3/right", "luz_for_enthium"],
        ["42keebs/cantor_pro/v3/left", "luz_for_colemak_dh"],
        ["42keebs/cantor_pro/v3/right", "luz_for_colemak_dh"],
        ["42keebs/cantor_pro/v3/left", "luz_for_qwerty"],
        ["42keebs/cantor_pro/v3/right", "luz_for_qwerty"]
    ]
}
```

## Compile Commands

Build all targets:
```sh
qmk userspace-compile
```

Build a single target:
```sh
qmk compile -kb kaly/kaly42 -km luz_for_gallium
qmk compile -kb kaly/kaly42 -km zen
qmk compile -kb 42keebs/cantor_pro/v3/left -km luz_for_gallium
qmk compile -kb 42keebs/cantor_pro/v3/left -km zen
qmk compile -kb kaly/kaly42 -km luz_for_colemak_dh
qmk compile -kb kaly/kaly42 -km luz_for_qwerty
```

## Key Position Reference (split_3x6_3)

```
┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
│  0 │  1 │  2 │  3 │  4 │  5 │       │  6 │  7 │  8 │  9 │ 10 │ 11 │
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│ 12 │ 13 │ 14 │ 15 │ 16 │ 17 │       │ 18 │ 19 │ 20 │ 21 │ 22 │ 23 │
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│ 24 │ 25 │ 26 │ 27 │ 28 │ 29 │       │ 30 │ 31 │ 32 │ 33 │ 34 │ 35 │
└────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
                  ┌────┬────┬────┐ ┌────┬────┬────┐
                  │ 36 │ 37 │ 38 │ │ 39 │ 40 │ 41 │
                  └────┴────┴────┘ └────┴────┴────┘
```

## Notes
- The Cantor Pro keyboard definition files live in the main QMK firmware repo (`~/qmk_firmware/keyboards/42keebs/cantor_pro/`), not in this userspace
- The Cantor Pro v3 uses RP2040 with double-tap reset bootloader
- Firmware output files are `.bin` for Kaly42 and `.uf2` for Cantor Pro
