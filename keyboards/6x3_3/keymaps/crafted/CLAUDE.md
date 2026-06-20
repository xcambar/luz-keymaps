# Keymap Drawer YAML Maintenance

## Purpose

Keep the per-layer YAML files in `keymap_drawer/` in sync with `keymap.c`, then build
the SVGs and the PDFs via `keymap_drawer/build_pdf.sh`. The YAMLs follow the
keymap-drawer spec (https://github.com/caksoylar/keymap-drawer).

## Workflow

1. **Read configuration first** — `rules.mk` (`XC_LAYOUT`, `XC_SECONDARY_LAYOUT`,
   `XC_WEAK_CORNERS`, `COMBO_ENABLE`) and `config.h`. Never ask for what these files state.
2. **Parse `keymap.c`** — `enum layers`, the `LAYOUT_split_3x6_3` blocks, `key_combos[]`,
   and `enum custom_keycodes` (in `custom_keycodes.h` and feature headers).
3. **Update the per-layer YAMLs** in `keymap_drawer/` (format below).
4. **Build**: run `./build_pdf.sh` from `keymap_drawer/` — it iterates `[0-9]*.yml`,
   producing one committed SVG per file plus merged landscape-A4 `crafted.pdf` (color) +
   `crafted_print.pdf` (flat B/W). PNGs are rendered only into the build temp dir as PDF
   intermediates (not committed). **Deps: Inkscape + Source Sans 3 font** (color path).

   The YAML CSS is only the *palette/semantics* source. The build layers two post-processes
   on top (don't duplicate these in YAML): `bake_corner_nudge` (CSS corner transform → SVG
   attribute, for ImageMagick) and `apply_design` (the "Direction A" look: Source Sans 3
   labels with mono kept for literal single-glyph keycodes, warm paper, floating keycap
   shadows, warm-neutral inactive keys, editorial title). Color SVGs are rasterized with
   **Inkscape** (the only backend that renders the font + shadow filter + `<use>` glyphs);
   the flat B/W print path stays on `convert`.

## Files

One self-contained YAML per *rendered* layer, numbered by render order:

```
keymap_drawer/00_BASE.yml      # Layer 0  (Gallium East)
keymap_drawer/01_SYMBOLS.yml   # Layer 3  (render order before FAVS by choice)
keymap_drawer/02_FAVS.yml      # Layer 2  (FAVS; sub-modes are in the README table, not on-key hints)
keymap_drawer/03_ADJUST.yml    # Layer 5
```

- **BASE_ALT (layer 1, the secondary QWERTY base) is deliberately not rendered.**
- Each file contains its own `layout:` (`qmk_keyboard: cantor`,
  `layout_name: LAYOUT_split_3x6_3`), `layers:`, and `draw_config:` block.
  Keep the `draw_config` identical across all files.
- When a layer is removed from keymap.c, delete its YAML *and* its generated
  `.svg` artifact; renumber if render order changes.

## Layers (from keymap.c)

| # | Layer    | Access                                          | Rendered |
|---|----------|--------------------------------------------------|----------|
| 0 | BASE     | default (Gallium East via `XC_LAYOUT`)           | yes      |
| 1 | BASE_ALT | default-layer toggle (`SW_LYT` on ADJUST)        | no       |
| 2 | FAVS     | hold left inner thumb `MO(FAVS)` (pos 38)        | yes      |
| 3 | SYMBOLS  | hold right inner thumb `MO(SYMBOLS)` (pos 39)    | yes      |
| 4 | NAV_DEL  | hold `Dl⊙` (pos 14) while on FAVS — hold-only    | no (per-key roles in the README modes table) |
| 5 | TABS     | hold tab trigger (pos 3) while on FAVS — hold-only | no (per-key roles in the README modes table) |
| 6 | ADJUST   | tri-layer: hold both inner thumbs (FAVS+SYMBOLS) | yes      |

## Key position reference (split_3x6_3)

```
Row 1:  0   1   2   3   4   5  |   6   7   8   9  10  11
Row 2: 12  13  14  15  16  17  |  18  19  20  21  22  23
Row 3: 24  25  26  27  28  29  |  30  31  32  33  34  35
Thumbs:            36  37  38  |  39  40  41
```

YAML rows: three rows of 12 keys, then a thumb row of 6.

## Key conversion rules

- `KC_A`–`KC_Z` → the letter; digits plain (`7`); punctuation quoted (`","`, `"."`)
- `KC_NO` / `XXXXXXX` → `""`
- `KC_TRNS` / `_______` → `{ t: "▽", type: trans }` **only when the fall-through chain
  reaches a real key**; `""` when it resolves to KC_NO. Resolve the chain with the
  layers actually active: ADJUST → SYMBOLS → FAVS → BASE.
- `MO(LAYER)` → `{ t: $$mdi:layers-outline$$, h: "LAYER (MO)", type: layer }`
- **Held keys**: on a layer reached by holding, mark the held position(s) with
  `{ type: held }` — nothing else, no `t:`/`h:`. FAVS marks pos 38 held;
  ADJUST has two (38 and 39).
- Mod-taps → `{ t: X, h: Mod, type: modtap }` (e.g., `LALT_T(KC_Q)` → `{ t: Q, h: LAlt, type: modtap }`).
  The `modtap` type highlights **only the hold legend** (key body stays plain); reserve
  `type: modifier` for keys that are modifiers outright (thumb Shift, SEL_LATCH, MM_GUICTRL).
- Shifted-pair customs (`SL_*`, `AS_*`, `XC_*`) → `{ t: x, s: y, type: symbol }`,
  e.g., `SL_LPRN` → `{ t: "(", s: "<" }`, `XC_UNDS` → `{ t: "_", s: "|" }`

### Custom keycodes in this keymap

| Keycode | Rendering |
|---------|-----------|
| `SW_WIN` | `$$mdi:swap-horizontal$$`, type system |
| `MM_GUICTRL` | `$$mdi:star-four-points-box$$`, h: "GUI/Ctrl", type modifier |
| `SEL_LATCH` | `$$mdi:apple-keyboard-shift$$`, h: "Latch", type modifier |
| `MO(NAV_DEL)` (FAVS pos 14) | `$$mdi:backspace-outline$$`, h: "hold", type `nav delhold` (blue bg, coral legend). Trigger for the NAV_DEL sub-layer; per-key roles live in the README "Navigation modes" table, **not** as on-key hints |
| `MO(TABS)` (FAVS pos 3) | `s: hold`, `t: $$mdi:tab$$`, `h: tabs`, type `nav tabmode mode` — blue `nav` key, purple legends via `.key.tabmode.tap, .key.tabmode.hold, .key.tabmode.shifted { fill: #9a5fa1 }` (legend-position selectors like `delhold`/`selmode` — **not** `:not(rect)`, which the color Inkscape PDF path silently drops), italic "hold" via `.key.mode.shifted`. Trigger for the TABS sub-layer; per-key roles in the README modes table |
| nav cluster (right hand) | each movement key carries **only** its movement glyph — no corner hints. The Select / Delete / Tabs sub-modes reuse these same positions, but those roles are documented in the README "Navigation modes" table, deliberately **not** as on-key glyphs (keeps the cluster legible). `delhint`/`selhint`/`tabhint` tokens + their CSS were removed when the hints were dropped |
| `QK_LLCK` | `$$mdi:lock-outline$$`, h: "Layer Lock", type layer |
| `CW_TOGG` | `$$mdi:format-letter-case-upper$$`, h: "Caps Word", type editing |
| `MD_FENCE` | t: ` ``` `, type symbol |
| `ARROW_OP` | `{ t: "->", s: "=>" }`, type symbol |
| `SW_OS` / `PR_OS` | "OS ⇄" / "OS ?", type system |
| `SW_LYT` / `PR_LYT` | "Lyt ⇄" / "Lyt ?", type system |
| `QK_BOOT` | `$$mdi:restart$$`, h: "Boot", type system |
| `SK_*` semantic keys | Undo/Cut/Copy/Paste (system); cursor-motion granularity uses one **media-triangle magnitude scale** so the *reach* reads at a glance — triangle count = distance: **char** = single triangle `◀ ▶ ▲ ▼` (U+25C0/25B6/25B2/25BC), **word** = double triangle `◀◀`/`▶▶` (jump), **line** begin/end = `⏮`/`⏭` (U+23EE/U+23ED, double-triangle-to-bar = "hit the wall"). **Render gotchas (Inkscape + Source Sans 3):** the plain rewind/fast-forward codepoints `⏪`/`⏩` (U+23EA/U+23E9) do NOT render — they fall back to "B"/"A" — so word uses two literal `◀`/`▶` glyphs, not `⏪⏩`. `⏮`/`⏭` *do* render. Line was deliberately NOT `⇤⇥` or `arrow-collapse` (read as the Tab glyph beside the TABS cluster). The cluster shows movement glyphs only; sub-mode roles (deletions, tab actions) reusing these positions are in the README modes table, not on-key hints (nav/editing) |

Semantic (`SK_*`) and dead (`DK_*`) keys are OS-aware; render their *meaning*, not chords.

## Combos (BASE only, `COMBO_ONLY_FROM_LAYER 0`)

| key_positions | Output | Note |
|---------------|--------|------|
| [2, 3]   | B   | weak corner (when `XC_WEAK_CORNERS`) |
| [8, 9]   | `{ t: "'", s: '"' }` | weak corner |
| [27, 28] | Z   | weak corner |
| [31, 32] | K   | weak corner |
| [37, 40] | Compose | both inner-ish thumbs; arms the accent compose (`align: bottom`) |

Combos go in `00_BASE.yml` only. Use `align:` only for non-adjacent positions.

## Icons (Material Design, `$$mdi:name$$`)

Tab `keyboard-tab` · Esc `keyboard-esc` · Space `keyboard-space` · Enter `keyboard-return`
· Backspace `backspace-outline` · Shift `apple-keyboard-shift` · Ctrl `apple-keyboard-control`
· GUI `apple-keyboard-command` · arrows `arrow-left/-down/-up/-right` · layers `layers-outline`
/ `layers-triple-outline` (ADJUST) · volume `volume-plus/-minus/-mute` · brightness
`brightness-7/-5` · PrtScr `monitor-screenshot` · Boot `restart`

Icon names are lowercase-with-hyphens. Use icons consistently — never mix icon and text
for the same key type across layers.

## Key type categories (color coding)

Do **not** type letters or digits. Categories and their CSS (the `svg_extra_style`
block carried in every YAML):

- `modifier` blue · `nav` green · `editing` orange · `layer` purple · `system` yellow
- `symbol` teal · `trans` (built-in keymap-drawer gray, used for ▽)
- `modtap` — no rect styling; only the hold legend is colored blue/bold (`.key.modtap.hold`)

```yaml
.key.TYPE rect { fill: #light-bg; stroke: #dark-border; }
.key.TYPE :not(rect) { fill: #dark-border; }
```

## YAML syntax gotchas

- Always a space after the colon inside flow maps: `{t: "|", s: "~"}` — missing it breaks parsing
- Quote `"="`, `";"`, `"'"`, `","`, `"."`, `"@"`, braces/brackets; don't quote plain letters,
  digits, or layer names
- Each non-thumb row must have exactly 12 entries; the thumb row exactly 6
- Combos use explicit `layers:` lists

## Verification

After any sync, run `./build_pdf.sh` and confirm it completes; visually check changed
SVGs. The firmware itself is verified separately with
`qmk compile -kb 42keebs/cantor_pro/v3/left -km crafted`.
