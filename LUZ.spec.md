# Luz — specification

> This is the **precise, implementation-facing spec** — the exact contract each variant is
> kept against (file paths, macros, positions, rules). For a human-friendly tour of *what Luz
> is and why*, read [`LUZ.md`](./LUZ.md) first; come here for the details.

**Luz** is a framework of shared conventions and features for alternative keyboard
layouts on the `split_3x6_3` (42-key) form factor. It defines a common interaction
model — layer set, mod placement, navigation, symbols, compose — that stays identical
across layouts, while each layout keeps its own character of alpha key assignments.

Luz ships as layout-specific **variants**:

| Variant            | Layout       | Keymap directory                      |
|--------------------|--------------|----------------------------------------|
| **Luz for Gallium**| Gallium East | `keyboards/6x3_3/keymaps/luz_for_gallium`      |
| **Luz for Enthium**| Enthium      | `keyboards/6x3_3/keymaps/luz_for_enthium`      |

A *variant* differs from every other variant in exactly one place: **layer 0 (BASE)**.
Everything above BASE is Luz, and is shared verbatim.

---

## The shared layer model

This is the first and load-bearing Luz convention: a fixed set of six layers with fixed
roles, ordering, and activation. The enum is defined once, in the canonical shared header
[`keyboards/6x3_3/luz/layers.h`](keyboards/6x3_3/luz/layers.h). Each variant's `rules.mk`
adds the shared dir's parent to the include path (`VPATH += $(QMK_USERSPACE)/keyboards/6x3_3`)
and `#include`s it as `"luz/layers.h"`. No variant redefines the enum; renaming a layer
happens in one file.

| # | Layer         | Role                                            | Activation                                  | Shared |
|---|---------------|-------------------------------------------------|---------------------------------------------|--------|
| 0 | `BASE`        | The layout's alpha layer                        | default                                     | structure only — keys are layout-specific |
| 1 | `EXTEND`      | Navigation + shortcut hub; launches sub-layers  | hold left inner thumb — `MO(EXTEND)` (38)   | yes |
| 2 | `SYMBOLS`     | Numpad (left) + symbol field (right)            | right inner thumb — `LT(SYMBOLS, KC_ENT)` (39) | yes |
| 3 | `EXTEND_DEL`  | Deletion sub-layer (destructive)                | hold-only `MO(EXTEND_DEL)` from EXTEND (14)  | yes |
| 4 | `EXTEND_TABS` | Browser tab management sub-layer                | hold-only `MO(EXTEND_TABS)` from EXTEND (15) | yes |
| 5 | `ADJUST`      | F-keys, media, brightness, `QK_BOOT`            | tri-layer: EXTEND + SYMBOLS held together   | yes |

### Naming conventions

- **`BASE`** is the only layout-specific layer. It carries the underlying alpha layout
  (Gallium East, Enthium, …). Its *structure* — where the thumb triggers, mods, and
  privileged symbols sit — is fixed by Luz; only the alpha glyphs change.
- **`EXTEND`** is the navigation + shortcut hub ("extend layer"): arrows, paging,
  line/word motion, clipboard, window switching, and the launch points for the
  sub-layers.
- **Sub-layers carry the `EXTEND_` prefix** (`EXTEND_DEL`, `EXTEND_TABS`) to advertise
  that they are reached *from* EXTEND and are scoped to it. This makes the hierarchy
  self-documenting in the enum.
- **`SYMBOLS`** and **`ADJUST`** keep their conventional QMK-community names.

### Activation conventions

- **Two base overlays, two inner thumbs.** EXTEND is a momentary hold on the left inner
  thumb (38); SYMBOLS is a layer-tap (hold = layer, tap = Enter) on the right inner
  thumb (39).
- **Sub-layers are hold-only and launched from EXTEND.** `EXTEND_DEL` and `EXTEND_TABS`
  are momentary — never latched — and mutually exclusive with the EXTEND select latch.
  A destructive layer you must actively hold cannot be left on by accident.
- **`ADJUST` is the tri-layer** (both inner thumbs held → `update_tri_layer_state`) and is
  the **only** bootloader path (`QK_BOOT`).

### Structural rules

1. **Layer order is contractual.** The two base overlays (EXTEND, SYMBOLS) come first,
   then the sub-layers, then ADJUST. Sub-layers and ADJUST must sit *above* the overlays
   so their `KC_TRNS` fall-through chains resolve in the intended order
   (`ADJUST → SYMBOLS → EXTEND → BASE`).
2. **Cross-layer consistency.** The same output lives at the same physical position on
   every layer it appears, even at the cost of an empty slot elsewhere. A glyph never
   migrates between layers.
3. **Privileged base keys fall through.** Keys placed on the outer thumbs and envelope
   (Esc, Enter, the privileged base symbols) stay reachable on overlay layers via
   transparency rather than being re-declared.

---

## The shared symbol set

Symbols are split the same way the layers are: the **vocabulary and behavior** are a Luz
contract shared verbatim, while **placement** is where each variant keeps its character.
The shared half lives in [`keyboards/6x3_3/luz/symbols.h`](keyboards/6x3_3/luz/symbols.h)
(on the include path, like `layers.h`); placement lives in each `keymap.c`.

### Shared (the contract)

- **A single canonical set of symbol keycodes** (`SY_*`), defined by one table
  (`SYMBOL_TABLE`) that is the only source of truth. Each row generates both the keycode
  enum and the key-override array — add a symbol in one place and it exists in every
  variant.
- **Related-shift pairing.** Every symbol's unshifted glyph is `KC_<NAME>`; its Shift
  yields a *related* symbol rather than the QWERTY default — `(`→`<`, `{`→`[`, `=`→`+`,
  `&`→`*`, `,`→`?`, … This pairing is the muscle-memory-transferable part of Luz.
- **All-layer override scope.** The unshifted/shifted behavior is delivered by key
  overrides live on every layer (`~0`), so a symbol behaves identically wherever it is
  physically placed — nothing gates it by layer.
- **Mod-tap shift handling.** A variant may place a punctuation symbol on a BASE mod-tap;
  since a mod-tap's tap can't ride a key override, `SYM_MODTAP_SHIFT` supplies the shifted
  partner from `process_record_user`, reading the same `SY_*_SHIFTED` constants the table
  uses. The mechanism is shared; *which* symbols sit on mod-taps is per-layout.

### The pairs

The canonical set — 16 symbols, each `tap` → `shift`. This is the whole contract; adding or
removing a row here changes every variant.

| Keycode    | Tap | Shift | | Keycode    | Tap | Shift |
|------------|-----|-------|-|------------|-----|-------|
| `SY_QUOT`  | `'` | `"`   | | `SY_EQL`   | `=` | `+`   |
| `SY_MINS`  | `-` | `_`   | | `SY_DLR`   | `$` | `%`   |
| `SY_SLSH`  | `/` | `\|`  | | `SY_AMPR`  | `&` | `*`   |
| `SY_COMM`  | `,` | `?`   | | `SY_BSLS`  | `\` | `^`   |
| `SY_DOT`   | `.` | `!`   | | `SY_LPRN`  | `(` | `<`   |
| `SY_AT`    | `@` | `#`   | | `SY_RPRN`  | `)` | `>`   |
| `SY_GRV`   | `` ` `` | `~` | | `SY_LCBR`  | `{` | `[`   |
|            |     |       | | `SY_RCBR`  | `}` | `]`   |
|            |     |       | | `SY_COLN`  | `:` | `;`   |

Notes on the pairing logic: brackets group by kind so an unshifted glyph and its shifted
partner are the same *kind* of bracket (`(`/`<`, `{`/`[`, `}`/`]`); `:`/`;` and `-`/`_`
keep the two glyphs that share a physical key on a standard layout together; the rest pair a
symbol with a visually or semantically adjacent one (`=`/`+`, `&`/`*`, `@`/`#`, `` ` ``/`~`).

### Per-layout (placement) — and the principles that govern it

Placement differs between variants because each alpha layout frees up different positions
(and Enthium mirrors the hands), so the same symbol set is arranged differently. Placement
is free, but governed by these Luz **principles**:

1. **Privileged symbols sit on BASE positions; the rest live on SYMBOLS.** "Privileged" is
   purely a placement choice — the high-frequency punctuation a variant wants without a
   layer hold.
2. **Numpad on the left hand** (on SYMBOLS), keeping the right hand free for the mouse.
3. **Opening brackets on the index column**, with bracket pairs kept visually together
   (open over open, close over close).
4. **Symbols arranged by frequency** within the SYMBOLS field.
5. **Cross-layer consistency still applies:** a symbol that appears on both BASE and
   SYMBOLS sits at the same position on each.

---

## Compose

Luz produces accented and special characters through a **Compose** mechanism rather than
through extra layers or per-glyph keys. It is *armed by a chord and consumed by the next
keystroke*. Being able to write accents, diacritics and other common symbols is treated as a first-class concern of the framework.

### How it works

- **Arming:** tap the two **middle thumbs together** — `Shift` (pos 37) + `Space` (pos 40).
  This is the only combo in Luz. It sets a one-shot "compose
  pending" state; the thumbs otherwise remain plain Shift and Space.
- **Consuming:** the next key picks the result, then compose disarms:

  | Key | Result |        | Key | Result |
  |-----|--------|--------|-----|--------|
  | `E` | acute — `é`     || `C` | `ç` |
  | `A` | grave — `à`     || `N` | `ñ` |
  | `U` | diaeresis — `ü` || `W` | `€` |
  | `O` | circumflex — `ô`|| `Esc` | cancel |

  `E`/`A`/`U`/`O` emit **dead keys** (the accent then combines with the next letter, so
  `compose → E → a` gives `á`); `C`/`N`/`W` emit the character directly. **Plain modifiers
  don't consume compose**, so a held Shift still composes — `compose → Shift+E → a` →
  `Á`. **Any unmapped key passes through unchanged**, disarming compose.

Compose covers the high-frequency accents; the long tail it deliberately omits (e.g. `ß`)
stays reachable through **`RAlt` (AltGr) on the bottom-row mods**, which produces the OS
layout's AltGr characters directly — so nothing is locked out, Compose just fast-paths the
common cases.

### Why mnemonics, not positions

The handler matches on **letter keycodes** (`KC_E`, `KC_A`, …), not physical positions. The
mnemonics are chosen by meaning — **E** for the most common acute, **C** for cedilla — so they are identical across every variant even
though those letters sit at different positions in each layout. Compose behavior is shared
verbatim (the handler is byte-identical between variants).

---

## The mod system

Modifiers are placed by **position, not by letter** — every variant carries the same mod on
the same physical key, so chording for shortcuts is identical muscle memory across layouts.
The only thing that changes per layout is the alpha under each mod, which is just BASE. The
positional, shareable parts live in
[`keyboards/6x3_3/luz/mods.h`](keyboards/6x3_3/luz/mods.h) (on the include path, like `layers.h`).

### The scheme

- **Bottom-row mod-taps**, mirrored `Alt – GUI – Ctrl` from the outer column inward: left
  `26=Alt 27=GUI 28=Ctrl`, right `31=Ctrl 32=GUI 33=Alt`. Keeping the full trio off the home
  row leaves the home keys clean for typing.
- **Home-row index morph** (pos 16 / 19): a GUI mod-tap that is the unified **Cmd/Ctrl
  shortcut key** — GUI on macOS, **Ctrl on Linux** (the hold is rewritten by
  `LUZ_INDEX_GUI_MORPH`). This is what makes `⌘C`/`^C` and friends the same chord on both OSes.
- **Thumbs carry no bottom-row-style mod-taps**; the only thumb modifier is plain `Shift` (37).

### Shared (the contract) → `luz/mods.h`

- **Chordal Hold handedness array** (`chordal_hold_layout`) — purely positional (`L`/`R`/`*`,
  thumbs exempt), so it is defined once here and identical for every variant.
- **The Cmd/Ctrl morph** — `LUZ_INDEX_GUI_MORPH(keycode, record, morph_l, morph_r)`, a macro
  that expands inside `process_record_user` (same idiom as `SYM_MODTAP_SHIFT`): on a non-macOS
  platform it registers Ctrl for the held index morph keys. The per-layout `morph_l`/`morph_r`
  snapshots stay in `keymap.c` because they wrap that layout's index letter.
- **Tap-hold tuning** (in each `config.h`, identical, part of the contract): `TAPPING_TERM 240`,
  `CHORDAL_HOLD`, `PERMISSIVE_HOLD`, `FLOW_TAP_TERM 150`. Chordal Hold's opposite-hands rule
  prevents same-hand roll misfires; Flow Tap suppresses holds during fast typing bursts.

### Per-layout

Only the letters under the mods — i.e. nothing but BASE. A variant must place its alphas so the
mod *positions* above still hold (index morph on the index home keys; the Alt/GUI/Ctrl trio on
the bottom row). Enthium mirrors the hands, but because the scheme is itself mirror-symmetric,
every letter keeps the same modifier under the opposite hand.

---

## The thumb cluster — *deliberately not a convention*

Luz imposes **nothing** on the thumb cluster beyond what other conventions already require:
the layer model pins the two inner thumbs (EXTEND on the left inner, SYMBOLS layer-tap on the
right inner), and the mod system puts a plain `Shift` on one thumb. Everything else — the outer
thumbs especially — is **free per variant**. Crafted uses `Esc`/`Enter` on the outer thumbs;
Enthium puts `R` on a thumb to free a base slot and moves `Esc` accordingly. That divergence is
intended, not drift: the thumbs are where a layout spends its spare keys, so Luz leaves them open.

---

## The navigation cluster

EXTEND's **right hand is a modifier-free navigation cluster**; its **left hand hosts the
controls** and the launch points for three sub-modes. Luz fixes the cluster's *shape* and the
sub-mode *behavior*; the commands that fill them are **suggested, not imposed** (see below).

### The cluster (shape — the convention)

- **WASD-style inverted-T arrows** on the right hand (`↑` + `← ↓ →`), kept **modifier-free** —
  no home-row/oneshot mods on the nav layer, so motions stay bare cursor keys (this matters for
  terminal simultaneity and clean mouse-gesture interplay).
- **Magnitude grows away from the home row:** character motion on the home arms, **line** motion
  on the row above (flanking `↑`), **word** motion on the row below; **paging** as a vertical
  pair on the inner column. Distance from home reads as distance moved.

### The sub-modes (behavior — the convention)

Three sub-modes launch from EXTEND's left home row and **reuse the very same right-hand cursor
cluster**, layering a role onto a spatial vocabulary you already know:

1. **Select — tap-latch.** Tap `Sl⊙` to hold a real Shift scoped to EXTEND; every cursor motion
   now extends a selection. Tap again, press `Esc`, or leave EXTEND to release. A *latch*, not a
   hold, so both hands stay free.
2. **Delete — hold-only.** Hold `Dl⊙` for the `EXTEND_DEL` sub-layer. Vim-like operator grammar:
   **row = granularity** (line / char / word), each deletion sitting on the motion it consumes.
   EXTEND's motions and clipboard stay live underneath via transparency.
3. **Tabs — hold-only.** Hold the tab trigger for `EXTEND_TABS`: browser tab management on the
   same inverted-T — index column = tab lifecycle (new / close / reopen), home-row arms = switch,
   plus history back/forward.

Rules: Delete and Tabs are **hold-only** (momentary, never latched — a destructive or contextual
mode can't be left on by accident); the sub-modes are **mutually exclusive** (a delete hold wins
over the select latch, and the latch drops on leaving EXTEND).

### Suggested, not imposed

What *fills* the cluster — the `SK_*` semantic keys (OS-aware clipboard, line/word motion,
deletions, tab actions) — is a **suggested default set, not a requirement**. A variant may swap
them, and every **free/empty EXTEND position is open for per-layout functions**. Luz fixes the
cluster's geometry and the sub-mode mechanics; what rides on top is the layout's to choose.

---

## Status

- [x] **Layer model** — enum, naming, activation, structural rules.
- [x] **Symbol set** — shared `SY_*` vocabulary + behavior (`luz/symbols.h`); placement
  per-layout under shared principles.
- [x] **Compose** — chord-armed dead-key/diacritic system; the sole Luz combo + the combo rules.
- [x] **Mod system** — positional mod placement; shared Chordal Hold + Cmd/Ctrl morph
  (`luz/mods.h`); tap-hold tuning contract.
- [x] **Navigation cluster** — modifier-free inverted-T + three reuse-the-cluster sub-modes
  (Select latch / Delete hold / Tabs hold); fills are suggested, not imposed.
- [x] **Thumb cluster** — *decided non-convention:* deliberately free per variant.

Conventions take whichever form fits them. Some are backed by shared code (`luz/layers.h`,
`luz/symbols.h`, `luz/mods.h`) so drift is impossible. Others are documentation contracts by
design — the navigation cluster fixes a *shape and behavior* but deliberately leaves the fill
to each variant, so imposing code would remove intended latitude. And some things, like the
thumb cluster, are deliberately left free. In every case this document is the authority the
variants are kept against.
