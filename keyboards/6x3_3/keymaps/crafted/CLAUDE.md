# Keymap Drawer YAML Generator Agent

## Purpose
This agent reads the `keymap.c` file, analyzes the keyboard layout using the ASCII art illustrations in comments, and generates a valid YAML file compatible with https://keymap-drawer.streamlit.app/ following the specification at https://github.com/caksoylar/keymap-drawer/blob/v0.22.1/KEYMAP_SPEC.md

## Agent Behavior
The agent is triggered whenever a change to the keymap is required. The agent:
- **MUST read configuration files** (`rules.mk`, `config.h`) to determine settings instead of asking questions
- **Fails gracefully** when lacking necessary resources or tools to generate the YAML
- **Asks questions only** when configuration is missing or invalid
- **Can generate YAML on demand** or automatically after keymap changes
- **Uses shorthand notation** for cleaner YAML output (`t:` for tap, `s:` for shifted, `h:` for hold)

## Input Sources
- **Source file**: `keymap.c` in the same directory
- **Configuration**: `rules.mk` (required), `config.h` (optional)
- **Key information sources**:
  - ASCII art layout illustrations in C comments
  - Layer definitions in `keymaps[][MATRIX_ROWS][MATRIX_COLS]` arrays
  - Combo definitions in `combo_t key_combos[]` array
  - Custom keycode definitions in `enum custom_keycodes`

## Output
A YAML file named `keymap.yaml` that can be uploaded to https://keymap-drawer.streamlit.app/

## Generating Visualizations

After updating `keymap.yaml`, generate the visual outputs using `keymap-drawer`:

**Using uvx (recommended)**:
```bash
# Generate SVG
~/.local/bin/uvx --from keymap-drawer keymap draw keymap.yaml > keymap.svg

# Generate PNG
~/.local/bin/uvx --from keymap-drawer keymap draw keymap.yaml -o keymap.png
```

**Note**: The executable is `keymap`, not `keymap-drawer`, when using uvx.

## Configuration Reading Rules

### MUST Read Before Generation:
1. **`rules.mk`** - Check for:
   - `XC_WEAK_CORNERS = yes/no` - Determines if weak corner keys are enabled
   - `COMBO_ENABLE = yes/no` - Determines if combos should be included
   - `XC_LAYOUT` - Determines which layout is active (qwerty, gallium, focal, graphite)
   - `XC_ALT_SYMBOLS_LAYER = yes/no` - Determines if alternative symbols layer is enabled

2. **`config.h`** (if exists) - Check for:
   - Tapping term settings
   - Combo term settings
   - Any layer-specific configurations

### Decision Logic:
- **Combos**: Use `COMBO_ENABLE` value from `rules.mk`
  - `yes` → Include combos section in YAML
  - `no` → Omit combos section
- **Weak Corners**: Use `XC_WEAK_CORNERS` value from `rules.mk`
  - `yes` → Weak corner keys are replaced by combos
  - `no` → Show actual keys in corner positions
- **Layout**: Use `XC_LAYOUT` value from `rules.mk` to determine which layout file is active
- **Alternative Symbols Layer**: Use `XC_ALT_SYMBOLS_LAYER` value from `rules.mk`
  - `yes` → Use alternative symbols layout with shifted symbols (e.g., 1→@, 2→$, {→[, }→], :→;, |→~)
  - `no` → Use default symbols layout with standard number row

## YAML Structure Requirements

Based on the keymap-drawer specification, the output must include:

### 1. Layout Definition
```yaml
layout:
  qmk_keyboard: kaly/kaly42
```

**Note**: Use the QMK keyboard identifier, not manual ortho_layout specification.

### 2. Layers
For each layer (0-3), create entries with proper key mappings:
```yaml
layers:
  Base:  # Layer 0
    - ["", "", W, E, R, T, Y, U, I, O, "", ""]
    - [Tab, A, S, D, F, G, H, J, K, L, ";", Bsp]
    - ["", Z, X, C, V, LCtl, RCtl, M, ",", ".", "/", ""]
    - ["", Sft, {h: FAVS}, Bsp, Spc, ""]
```

**Note**: Each row must have exactly 12 keys, thumb row has 6 keys (3 per side).

### 3. Special Key Types
Handle these QMK constructs using **shorthand notation**:

- **Mod-Tap keys** (`MT(MOD_X, KC_Y)`): Use `{t: Y, h: X}` format
  - Example: `MT(MOD_LGUI, KC_ESC)` → `{t: Esc, h: GUI}`
  - Shorthand: `t` = tap, `h` = hold

- **Layer access** (`MO(n)`, `TG(n)`, `TO(n)`):
  - `MO(2)` → `{h: NAV, type: layer}` (momentary - hold to activate layer)
  - `TG(3)` → `{h: NUM, type: layer}` (toggle - tap to toggle layer on/off)
  - `TO(0)` → `{t: Base, type: layer}` (switch to layer - **CRITICAL: use plain text in `t:`, NOT `{to: Base}`**)

  **CRITICAL RULE - Held Key Marking**:
  - **ALWAYS** mark hold keys on their target layer with `type: held`
  - When a key uses `{h: LAYER_NAME}` to access a layer via hold (MO, LT, or even TG shown as hold):
    1. On the source layer: `{h: NAV, type: layer}`
    2. On the target layer (NAV): at the same position, mark as `{type: held}`
  - **CRITICAL**: A held key NEVER has tap (`t:`) or hold (`h:`) contents - only `type: held`
  - The `held` type applies red shading to visually indicate the key must be held to remain on this layer
  - This applies to ALL hold-to-activate layer keys, not just MO()

  Example:
  ```yaml
  # Base layer
  - [Esc, Shift, { h: NAV, type: layer }, Bsp, Spc, Ent]

  # NAV layer (same thumb position)
  - ["", "", { type: held }, "", "", ""]
  ```

- **Custom dual-function keys with shift** - Use `{t: normal, s: shifted}`:
  - `BSP_DEL` → `{t: Bsp, s: Del}`
  - `PLS_MIN` → `{t: +, s: "-"}`
  - `MUL_DIV` → `{t: "*", s: /}`
  - `DOT_COM` → `{t: ., s: ","}`
  - `PARENS` → `{t: "(", s: ")"}`
  - `BRACES` → `{t: "{", s: "}"}`
  - `BRACKETS` → `{t: "[", s: "]"}`
  - `ANGLES` → `{t: <, s: ">"}`
  - `SLASHES` → `{t: /, s: "\\"}`
  - `QUOTE_DBL` → `{t: "'", s: "\""}`
  - `EQUAL_EXCL` → `{t: =, s: "!"}`
  - `TILDE_CARET` → `{t: "~", s: ^}`
  - `AMP_DOLLAR` → `{t: "&", s: $}`
  - `PIPE_TICK` → `{t: "|", s: "`"}`
  - `MINUS_UNDER` → `{t: "-", s: _}`
  - Shorthand: `s` = shifted

### 4. Combos
Extract from `combo_t key_combos[]` array. Parse the combo definitions and their associated layers.

**Combo alignment rules**:
- **ONLY use `align` property for non-adjacent keys** (e.g., when combo keys are separated)
- **DO NOT use `align` for adjacent keys** (keymap-drawer automatically positions these correctly)
- Adjacent combos don't need `align`
- Non-adjacent combos (e.g., thumb + home row) need `align: top` or `align: bottom`

**Critical combo parsing rules**:
- **Custom keycodes in combos**: When a combo definition uses a custom keycode (like `SFT_LEAD`), find the physical position where that custom key is placed in the keymap layout
  - Example: `const uint16_t PROGMEM sft_d_combo_l0[] = {SFT_LEAD, XC_D, COMBO_END};`
  - Find where `SFT_LEAD` is in the layout (position 37 in thumb row)
  - Find where `XC_D` or `KC_D` is in the layout (position 15)
  - Create combo with `key_positions: [37, 15]`
- **Multi-layer combos**: Parse the combo array names that end with layer indicators (e.g., `_l0`, `_l3`, `_l4`)
  - `sft_d_combo_l0` → applies to layer 0 (Base)
  - `sft_d_combo_l3` → applies to layer 3 (NUM)
  - `sft_d_combo_l4` → applies to layer 4 (SYM)
  - Create separate YAML combo entries for each layer, even if they use the same positions
- **Position consistency**: The key positions are the physical positions, but the keys at those positions may differ per layer
  - Position 15 is `D` on Base layer, `-` on NUM layer, `-` on SYM layer
  - The combo uses the same physical positions across layers, but triggers from different visual keys

## Key Position Mapping
For the split 3x6+3 layout, positions are:
```
Row 1:  0   1   2   3   4   5  |   6   7   8   9  10  11
Row 2: 12  13  14  15  16  17  |  18  19  20  21  22  23
Row 3: 24  25  26  27  28  29  |  30  31  32  33  34  35
Thumbs:            36  37  38  |  39  40  41

Left hand:
  Row 1: 0-5
  Row 2: 12-17
  Row 3: 24-29
  Thumbs: 36, 37, 38

Right hand:
  Row 1: 6-11
  Row 2: 18-23
  Row 3: 30-35
  Thumbs: 39, 40, 41
```

## Layer Name Mapping
```
0 → Base
1 → FAVS (Favorites/Modifiers)
2 → NAV (Navigation)
3 → SYMBOLS (Symbols and Numbers)
```

## Processing Steps

1. **Read keymap.c** - Parse the entire file
2. **Extract layout info** - Use `LAYOUT_split_3x6_3` arrays
3. **Parse illustrations** - Use ASCII art comments for visual reference
4. **Identify key types**:
   - Simple keys: `KC_A` → `A`
   - Transparent: `KC_TRNS` → `{ t: "▽", type: trans }` when it falls through to a real key on a lower layer; `""` when the fall-through resolves to KC_NO
   - No-op: `KC_NO` → `""`
   - Custom keycodes: Look up in `enum custom_keycodes` comments (OS_SHFT, OS_CTRL, OS_ALT, OS_GUI, SW_WIN, MM_GUICTRL)
   - Layer keys: Parse `MO(n)`, `TO(n)`, `QK_LAYER_LOCK`
5. **Extract combos** - Parse `combo_t key_combos[]` array:
   - Parse each combo definition array (e.g., `const uint16_t PROGMEM we_combo[] = {KC_W, KC_E, COMBO_END};`)
   - Map keycodes to physical positions by finding them in the layout arrays
   - For custom keycodes (e.g., `SFT_LEAD`, `XC_D`), search the layout to find their positions
   - Parse the `key_combos[]` array to get the combo action and determine which layers it applies to
   - Multi-layer combos have separate array definitions with layer suffixes (e.g., `_l0`, `_l3`, `_l4`)
   - Create separate YAML entries for each layer variant of a combo
6. **Generate YAML** - Format according to spec

## Special Considerations

- **Custom keycodes**: Oneshot modifiers (OS_SHFT, OS_CTRL, OS_ALT, OS_GUI), window swapper (SW_WIN), and morphing modifier (MM_GUICTRL)
- **Empty positions**: Use empty string `""` for KC_NO; mark KC_TRNS with `{ t: "▽", type: trans }` if a lower layer provides a real key at that position (resolve the fall-through chain), else `""`
- **Combo positions**: Calculate from the visual layout (0-indexed, left-to-right, top-to-bottom)
- **Weak corners**: When XC_WEAK_CORNERS is enabled, corner positions show placeholder keys with actual characters produced via combos

## Material Design Icons

keymap-drawer supports Material Design Icons using `$$mdi:ICON_NAME$$` syntax. **Always use icons for these keys**:

- **Shift** (LSft, RSft) → `$$mdi:apple-keyboard-shift$$`
- **Control** (LCtl, RCtl) → `$$mdi:apple-keyboard-control$$`
- **GUI/Command** (LGUI, RGUI) → `$$mdi:apple-keyboard-command$$`
- **Tab** → `$$mdi:keyboard-tab$$`
- **Escape** → `$$mdi:keyboard-esc$$`
- **Space** → `$$mdi:keyboard-space$$`
- **Enter/Return** → `$$mdi:keyboard-return$$`
- **Backspace** → `$$mdi:backspace-outline$$`
- **Arrow keys**:
  - **Up** → `$$mdi:arrow-up$$`
  - **Down** → `$$mdi:arrow-down$$`
  - **Left** → `$$mdi:arrow-left$$`
  - **Right** → `$$mdi:arrow-right$$`
- **Leader key** → `$$mdi:star$$` (reserved for future leader key implementation)
- **Held key indicator** → `$$mdi:circle-slice-8$$` (marks keys that must be held to remain on current layer)

**Icon usage rules**:
1. Icons work in both simple keys and dual-function keys
2. For dual-function: `{t: $$mdi:backspace-outline$$, s: Del}`
3. Icon names must be lowercase with hyphens (e.g., `apple-keyboard-shift`, not `Apple-Keyboard-Shift`)
4. **IMPORTANT**: Always use icons consistently - never mix icon and text for the same key type (e.g., don't use `RSft` when other shifts use the icon)
5. **CRITICAL**: When marking a key as `type: held`, ALWAYS use `$$mdi:circle-slice-8$$` as the hold value

## Key Styling with Types and CSS

### Key Type Categories

Assign `type` attribute to non-alphanumeric keys for color-coding. **Do NOT assign types to numbers (0-9) or letters (A-Z)**.

Categories:
1. **modifier** - Modifiers (Shift, Control, GUI, Alt)
2. **nav** - Navigation (arrows, Home, End, PgUp, PgDn, Tab)
3. **editing** - Editing/Actions (Backspace, Delete, Enter, Escape, Space)
4. **layer** - Layer switching (NAV, NUM, SYM, ACC, Base)
5. **system** - System commands (Copy, Paste, Cut, Undo, Redo via C-C, C-V, etc.)
6. **symbol** - Symbols and punctuation (all dual-function symbol keys, `;`, `'`, etc.)
7. **accent** - Accented characters (RA-', RA-N, RA-C, etc.)

### Example Usage
```yaml
{ t: $$mdi:apple-keyboard-shift$$, type: modifier }
{ t: $$mdi:arrow-left$$, type: nav }
{ t: +, s: "-", type: symbol }
{ t: Base, type: layer }
```

### CSS Styling Pattern

In `draw_config.svg_extra_style`, use this pattern for each category:

```yaml
svg_extra_style: |
  /* Category name - Color */
  .key.typename rect { fill: #light-bg; stroke: #dark-border; }
  .key.typename :not(rect) { fill: #dark-border; }
```

**How it works**:
- `.key.typename rect` - Targets the key rectangle background
  - `fill` - Light pastel background color
  - `stroke` - Darker border color
- `.key.typename :not(rect)` - Targets everything except the rectangle (text, shadows, icons)
  - `fill` - Solid color matching the border

**Color scheme** (current):
- modifier: Blue (#d6e5f5 bg, #4a7ab8 border)
- nav: Green (#d9f0d9 bg, #5ca75c border)
- editing: Orange (#ffe4d1 bg, #e88a3a border)
- layer: Purple (#ead9eb bg, #9a5fa1 border)
- system: Yellow (#fff4d1 bg, #e8b52a border)
- symbol: Cyan/Teal (#d4f0e8 bg, #4aa78a border)
- accent: Pink/Coral (#ffe0d6 bg, #e86d4a border)

## File Header

Always include at the top of keymap.yaml:
```yaml
# Keymap visualization tool: https://keymap-drawer.streamlit.app/
# Repository: https://github.com/caksoylar/keymap-drawer-web
```

## Critical YAML Syntax Rules (From Working Example)

**MUST follow these rules exactly:**

1. **Layer switching `TO(0)`**: Write as plain string `Base`, **NOT** `{to: Base}`
2. **Equals sign**: **MUST** quote as `"="` (YAML treats unquoted `=` as key:value)
3. **Spacing in objects**: **CRITICAL** - Always put space after colon:
   - ✅ Correct: `{t: "|", s: "`"}`
   - ❌ WRONG: `{t: "|", s:"`"}` (missing space after `s:` causes parse error)
4. **Combos format**: Always use explicit layers list:
   ```yaml
   - key_positions: [2, 3]
     key: Q
     align: top
     layers:
       - Base
   ```
5. **Quoting rules** (based on working example):
   - **Quote**: `"="`, `";"`, `"'"`, `","`, `"."`, punctuation
   - **Don't quote**: Numbers, simple letters, layer names (`Base`, `NAV`)
   - **In dual-function**: Quote special chars in both tap and shifted
6. **Slash**: Can be unquoted `/` as simple key, context-dependent

## Example Output Format

```yaml
layout:
  qmk_keyboard: kaly/kaly42

layers:
  Base:
    - ["", "", W, E, R, T, Y, U, I, O, "", ""]
    - [Tab, A, S, D, F, G, H, J, K, L, ";", Bsp]
    - ["", Z, X, C, V, "", "", M, ",", ".", "/", ""]
    - ["", Sft, {h: FAVS}, FAVS, Spc, ""]

  FAVS:
    - ["", "", "", "", "", "", "", "", "", "", "", ""]
    - [Esc, Alt, GUI, Ctrl, MM_GUICTRL, SW_WIN, Bsp, Del, Ent, "", "", ""]
    - ["", "", "", "", "", "", "", "", "", "", "", ""]
    - ["", OSf, {h: SYMBOLS}, {type: held}, OSf, ""]

  # ... continue for NAV and SYMBOLS layers

combos:
  - key_positions: [2, 3]
    key: Q
    layers: [Base]
  # ... continue for all combos

draw_config:
  key_h: 60
  combo_h: 22
  arc_scale: 1.0
```

## Instructions for Agent

### Mandatory Steps (in order):

1. **Read configuration files FIRST**:
   - Read `rules.mk` to get `XC_WEAK_CORNERS`, `COMBO_ENABLE`, and `XC_LAYOUT` values
   - Read `config.h` if it exists (optional)
   - **DO NOT ask about these settings** - read them from files

2. **Parse keymap.c**:
   - Read entire file
   - Parse ASCII art comments for visual reference
   - Extract all 4 layer definitions from `keymaps[][MATRIX_ROWS][MATRIX_COLS]`
   - Extract combos from `combo_t key_combos[]`
   - Note custom keycodes from `enum custom_keycodes`

3. **Map QMK to YAML**:
   - Use shorthand notation (`t:`, `h:`, `s:`)
   - Include combos only if `COMBO_ENABLE = yes`
   - Map custom keycodes (oneshot modifiers, swapper, mod morph)
   - Read the active layout from `layouts/` directory based on `XC_LAYOUT` setting

4. **Generate YAML**:
   - Use proper indentation (2 spaces)
   - Include all 4 layers: Base, FAVS, NAV, SYMBOLS
   - Add combo alignment (`align: top/bottom`)
   - Include `draw_config` section with sensible defaults

5. **Save output**:
   - Write to `keymap.yaml` in same directory
   - Ensure valid YAML syntax

6. **Generate visualizations**:
   - After saving `keymap.yaml`, generate SVG and PNG outputs using uvx
   - Run: `~/.local/bin/uvx --from keymap-drawer keymap draw keymap.yaml > keymap.svg`
   - Run: `~/.local/bin/uvx --from keymap-drawer keymap draw keymap.yaml -o keymap.png`

### Error Handling:
- If `rules.mk` is missing → Fail with error message
- If `keymap.c` is missing → Fail with error message
- If unable to parse a keycode → Ask for clarification
- If layer count doesn't match → Ask for clarification

## Notes
- This keyboard uses a split ortholinear layout (Kaly42)
- It has 4 layers with different purposes
- Base layer keys are defined in `layouts/` directory (qwerty.h, focal.h, gallium.h, or graphite.h)
- Active layout is determined by `XC_LAYOUT` setting in `rules.mk`
- Features oneshot modifiers for Shift, Ctrl, Alt, and GUI
- Window swapper (SW_WIN) for app switching
- Morphing modifier (MM_GUICTRL) that toggles between GUI and Ctrl
- Combos are conditional (read `COMBO_ENABLE` from `rules.mk`)
- Weak corners feature (read `XC_WEAK_CORNERS` from `rules.mk`) replaces corner keys with combos
- Always use shorthand notation for cleaner output

## Layer Structure
- **Layer 0**: Base - Main typing layer with letters and basic keys
- **Layer 1**: FAVS - Favorites layer with oneshot modifiers and common shortcuts
- **Layer 2**: NAV - Navigation with arrow keys and editing commands
- **Layer 3**: SYMBOLS - Numbers and symbols (layout varies based on XC_ALT_SYMBOLS_LAYER setting)

## Symbol Layer Variants

The SYMBOLS layer has two possible configurations based on the `XC_ALT_SYMBOLS_LAYER` setting in `rules.mk`:

### Default Layout (XC_ALT_SYMBOLS_LAYER = no)
Standard number row with symbols accessed via shift:
- Numbers: 1 2 3 4 5 6 7 8 9 0
- Shifted: ! @ # $ % ^ & * ( )
- Bottom row: ; = \ ' [ ] - , . /

### Alternative Layout (XC_ALT_SYMBOLS_LAYER = yes)
Alternative symbols as shifted values with inverted pairs:
- Numbers with alt symbols: 1→@ 2→$ 3→% 4→# 5→& 6→+ 7→= 8→* 9→_ 0→`
- Inverted symbols: {→[ }→] :→; |→~
- Other symbols: \→^ (→< )→>
