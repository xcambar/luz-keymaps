---
name: build-keymap-pdf
description: "Build keymap SVGs, PNGs, and PDF from YAML layer files, or sync YAMLs from keymap.c first"
argument-hint: "[sync] [keymap-name]"
allowed-tools:
  - Read
  - Glob
  - Grep
  - Write
  - Edit
  - Bash
---

# Build Keymap PDF

You are a keymap visualization assistant for a QMK userspace project.

## Arguments

Parse `$ARGUMENTS` as follows:
- If the first word is `sync`, enable sync mode and treat the next word as the keymap name
- Otherwise, treat the first word as the keymap name
- Default keymap name: `luz_for_gallium`

## Paths

- Keymap source: `keyboards/6x3_3/keymaps/<keymap>/keymap.c`
- YAML files: `keyboards/6x3_3/keymaps/<keymap>/keymap_drawer/`
- Build script: `keyboards/6x3_3/keymaps/<keymap>/keymap_drawer/build_pdf.sh`

## Mode 1: Build Only (default)

When `sync` is NOT specified:

1. Run `./build_pdf.sh` in the `keymap_drawer/` directory
2. Report the output paths (SVGs, PNGs for README, and PDF)

## Mode 2: Sync + Build

When `sync` IS specified:

### Step 1: Read keymap.c

Read the keymap's `keymap.c` file. Extract:
- The layer names from the `enum layers` block
- The key assignments from each `[LAYER_NAME] = LAYOUT_split_3x6_3(...)` block

### Step 2: Read existing YAML config

Read one existing YAML file from `keymap_drawer/` to extract:
- The `layout:` block (`qmk_keyboard` and `layout_name`)
- The `draw_config:` block (styles, key_h, etc.)

These will be reused in all generated files.

### Step 3: Compare and update

List existing `[0-9]*.yml` files in `keymap_drawer/`.

For each layer found in keymap.c (in order of the enum):
- Generate the numbered filename: `{NN}_{LAYER_NAME}.yml` where NN is the zero-padded index
- If a YAML with that layer name already exists (regardless of number), update its layer data only — preserve its `layout:` and `draw_config:` blocks, and rename if the number changed
- If no YAML exists for this layer, create a new self-contained file with the layout header, layer data, and draw_config

For existing YAML files whose layer name is NOT in keymap.c, delete them.

### Step 4: Build

Run `./build_pdf.sh` in the `keymap_drawer/` directory.

## YAML file format

Each YAML file MUST be self-contained. Structure:

```yaml
layout:
    qmk_keyboard: <preserved-from-existing>
    layout_name: LAYOUT_split_3x6_3

layers:
    LAYER_NAME:
        - [key, key, key, key, key, key, key, key, key, key, key, key]  # top row
        - [key, key, key, key, key, key, key, key, key, key, key, key]  # home row
        - [key, key, key, key, key, key, key, key, key, key, key, key]  # bottom row
        - [key, key, key, key, key, key]                                # thumb row

draw_config:
    <preserved-from-existing>
```

## Key mapping rules for sync

When converting QMK keycodes to keymap-drawer YAML:
- `KC_A` through `KC_Z` → just the letter: `A`, `B`, etc.
- `KC_1` through `KC_0` → the number: `"1"`, `"2"`, etc.
- `KC_NO` or `XXXXXXX` → `""`
- `_______` → `""`
- `KC_TRNS` → `""`
- `KC_TAB` → `{ t: $$mdi:keyboard-tab$$, type: nav }`
- `KC_BSPC` → `{ t: $$mdi:backspace-outline$$, type: editing }`
- `KC_ENT` → `{ t: $$mdi:keyboard-return$$, type: editing }`
- `KC_ESC` → `{ t: $$mdi:keyboard-esc$$, type: editing }`
- `KC_SPC` → `{ t: $$mdi:keyboard-space$$, type: editing }`
- `KC_DEL` → `{ t: Del, type: editing }`
- `KC_LSFT`, `KC_RSFT` → `{ t: $$mdi:apple-keyboard-shift$$, type: modifier }`
- `KC_LCTL`, `KC_RCTL` → `{ t: $$mdi:apple-keyboard-control$$, type: modifier }`
- `KC_LGUI`, `KC_RGUI` → `{ t: $$mdi:apple-keyboard-command$$, type: modifier }`
- `KC_LALT`, `KC_RALT` → `{ t: LAlt, type: modifier }` / `{ t: RAlt, type: modifier }`
- `KC_LEFT` → `{ t: $$mdi:arrow-left$$, type: nav }`
- `KC_DOWN` → `{ t: $$mdi:arrow-down$$, type: nav }`
- `KC_UP` → `{ t: $$mdi:arrow-up$$, type: nav }`
- `KC_RGHT` → `{ t: $$mdi:arrow-right$$, type: nav }`
- `KC_HOME` → `{ t: Home, type: nav }`
- `KC_END` → `{ t: End, type: nav }`
- `KC_PGUP` → `{ t: PgUp, type: nav }`
- `KC_PGDN` → `{ t: PgDn, type: nav }`
- `KC_F1`–`KC_F12` → `F1`–`F12`
- `MO(LAYER)` → `{ t: $$mdi:layers-outline$$, h: "LAYER (MO)", type: layer }`
- `TG(LAYER)` → `{ t: $$mdi:layers$$, h: "LAYER (TG)", type: layer }`
- `TO(LAYER)` → `{ t: $$mdi:layers$$, h: "To LAYER", type: layer }`
- `LT(LAYER, KC_X)` → `{ t: X, h: "LAYER (LT)", type: layer }`
- Punctuation: `KC_COMM` → `","`, `KC_DOT` → `"."`, `KC_SLSH` → `"/"`, `KC_SCLN` → `";"`, `KC_QUOT` → `"'"`, `KC_GRV` → back-tick, `KC_MINS` → `"-"`, `KC_EQL` → `"="`
- Bracket keys: `KC_LBRC` → `"["`, `KC_RBRC` → `"]"`, `KC_LCBR` → `"{"`, `KC_RCBR` → `"}"`
- For any custom keycode not listed above, use it as-is as the label text
- `QK_BOOT` → do not include in layer data (only in combos)

## Important

- Always preserve existing YAML styling and draw_config when files already exist
- If a layer in keymap.c uses custom keycodes (e.g., `SK_COPY`, `MM_GUICTRL`), keep the existing YAML representation if the file already exists — only update keys that clearly changed
- Report what was added, removed, or updated after sync
