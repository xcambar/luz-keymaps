# Luz — specification

> This is the **precise, implementation-facing spec** — the exact contract each variant is
> kept against (file paths, macros, positions, rules). For a human-friendly tour of *what Luz
> is and why*, read [the README](./README.md) first; come here for the details.

**Luz** is a framework of shared conventions and features for alternative keyboard
layouts on the `split_3x6_3` (42-key) form factor. It defines a common interaction
model — layer set, mod placement, navigation, symbols, compose — that stays identical
across layouts, while each layout keeps its own character of alpha key assignments.

Luz ships as layout-specific **variants**:

| Variant            | Layout       | Keymap directory                      |
|--------------------|--------------|----------------------------------------|
| **Luz for Gallium**| Gallium East | `keyboards/6x3_3/keymaps/luz_for_gallium`      |
| **Luz for Enthium**| Enthium      | `keyboards/6x3_3/keymaps/luz_for_enthium`      |
| **Luz for Colemak-DH**| Colemak Mod-DH (matrix) | `keyboards/6x3_3/keymaps/luz_for_colemak_dh` |
| **Luz for QWERTY**  | QWERTY       | `keyboards/6x3_3/keymaps/luz_for_qwerty`       |

A *variant* is a layout dropped into the Luz frame. What varies is deliberately small and
deliberately **enumerated** — a conformance checklist, not a vague licence:

| May vary | Must not vary |
|----------|---------------|
| BASE alpha placement | the layer set, their order, and how each is reached |
| which symbols are privileged onto BASE, and where | the symbol *vocabulary* and its shift pairings |
| the SYMBOLS right-hand field | the SYMBOLS left-hand numpad |
| BASE position 18's tap (it doubles as the right morph) | Shift plain on 37; the morph mirrored on 17/18 |
| what sits under a pinned hold — the morph's taps, and the EXTEND hold on 38 | mod positions: 37, 17/18, 26–28, 31–33, and the mod latch on SYMBOLS |
| *(nothing — Compose is fixed end to end)* | Compose on thumbs 37+40, Shift first, and everything it emits |
| EXTEND's fills (the `SK_*` set is a default) | the EXTEND cluster's geometry and sub-mode rules |

Everything else — EXTEND, EXTEND_DEL, EXTEND_TABS, ADJUST — is identical in every variant.
A variant that respects the right-hand column is a Luz variant, whatever firmware it is
written for.

---

## The shared layer model

This is the first and load-bearing Luz convention: a fixed set of six layers with fixed
roles, ordering, and activation. The enum is defined once, in the canonical shared header
[`keyboards/6x3_3/luz/layers.h`](keyboards/6x3_3/luz/layers.h). Each variant's `rules.mk`
includes the shared [`luz/rules.mk`](keyboards/6x3_3/luz/rules.mk), which adds the shared dir's
parent to the include path (`VPATH += $(QMK_USERSPACE)/keyboards/6x3_3`), so variants
`#include` it as `"luz/layers.h"`. No variant redefines the enum; renaming a layer
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
  are momentary — never latched — and mutually exclusive with the EXTEND Select hold.
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
   migrates between layers. Shift obeys this by staying *out* of the overlays entirely: the
   plain `KC_LSFT` at 37 is transparent everywhere above BASE, so it is at 37 on every layer.
   EXTEND carries a *second* Shift at position 16, which is an addition rather than a
   relocation — it is the Select-mode trigger, not a Shift you reach for to capitalise.
3. **Privileged base keys fall through.** The privileged BASE symbols *and the envelope*
   (Tab, Backspace) stay reachable on overlay layers via transparency rather than being
   re-declared. Note that the envelope's *positions* are per-variant — 12/23 in Gallium and
   Colemak-DH, 24/35 in Enthium — so the transparent positions differ between variants even
   though the rule does not. Getting this wrong is silent: the key simply isn't there. This no longer applies
   to the outer thumbs: 36 and 41 are blank, and what used to fall through from them is
   reachable elsewhere (`Esc` on EXTEND at 12, `Enter` as the tap of `LT(SYMBOLS)` at 39).

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
- **Mod-tap shift handling.** A variant may place a punctuation symbol on a BASE mod-tap
  (BASE is the only layer that carries mod-taps at all);
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
2. **Digits on the left hand** (on SYMBOLS), so a locked layer lets figures be keyed in with
   one hand and the other left on the mouse. `1 2 3` sit on the home row with `0` beside them,
   `4 5 6` below and `7 8 9` above — a calculator's rows with the middle and bottom swapped, so
   the digits typed most often are the ones on home.
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

- **Arming:** a single cross-hand thumb combo on BASE — **`Shift` (37) then `Space` (40)**. It
  sets a one-shot "compose pending" state; the member keys otherwise behave normally. Unlike
  every other Luz convention there is no per-variant half: the mod contract puts a plain
  `KC_LSFT` on 37 and Space on 40 in *every* variant, so the positions, the operands and the
  behaviour are all shared. The combo is declared outright in
  [`keyboards/6x3_3/luz/compose.h`](keyboards/6x3_3/luz/compose.h); a `keymap.c` only references
  it.

  A combo operand must be a key you never type in sequence with the other operand. Shift
  satisfies that by not being a typed character at all — which is what makes this pair safe,
  and what an alpha pair can never quite be.

  **`COMBO_MUST_PRESS_IN_ORDER` (Shift first) is part of the contract.** It closes the one
  sequence that could fire the chord by accident: rolling out of a Space into the next word's
  capital, right thumb still down while the left reaches for Shift, lands both keys inside
  `COMBO_TERM` and would otherwise arm Compose and swallow the space. Pressing Shift first is
  the natural way to make the gesture, so the constraint costs nothing.

  > [!NOTE]
  > Compose briefly lived on positions 5+6 with per-variant operands (`V`+`J`, `B`+`J`,
  > `X`+`=`). That was a workaround for Shift becoming a mod-tap on a letter — a design that was
  > explored and **not** adopted; the shipped rework put plain Shift back on the thumb. The
  > workaround outlived the problem it solved. Moving it back also retires the last per-variant
  > difference in what was otherwise shared verbatim.
- **Consuming:** the next key picks the result, then compose disarms:

  | Key | Result |        | Key | Result |
  |-----|--------|--------|-----|--------|
  | `E` | acute — `é`     || `N` | tilde — `ñ ã õ` |
  | `A` | grave — `à`     || `C` | `ç` |
  | `U` | diaeresis — `ü` || `W` | `€` |
  | `O` | circumflex — `ô`|| `Esc` | cancel |

  `E`/`A`/`U`/`O`/`N` emit **dead keys** (the accent then combines with the next letter, so
  `compose → E → a` gives `á`); `C`/`W` emit the character directly. `N` is a dead key rather
  than a literal `ñ` so the tilde reaches every base letter — `ã` and `õ` cost the same three
  keystrokes as `ñ`, where before they were unreachable and `ñ` was a special case. **Plain modifiers
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
- **Shift is a plain modifier on thumb 37** — not a mod-tap at all. `chordal_hold_layout`
  marks the thumbs `'*'` (exempt from the opposite-hands rule), so **one key holds for both
  hands**: the modifier you hold most often costs no tap-hold arbitration whatsoever, and needs
  no mirrored twin.
- **The home row carries no mod-taps.** Positions 15/16/19/20 are plain letters.
- **Cmd/Ctrl morph mirrored on the inner index column** — **17** (`LGUI_T`) and **18**
  (`RGUI_T`): the unified shortcut key, GUI on macOS and Ctrl on Linux, so `⌘C` and `^C` are the
  same chord. Mirrored because unlike Shift it *is* subject to the opposite-hands rule. Two
  reasons it lives at 17/18 rather than the home row:
    1. 17/18 is the lightest pair in every layout (~0.8–4.4% of English letters), because the
       inner index column is where layouts put their least frequent keys.
    2. More sharply, the index home pair spans a common **cross-hand bigram** in two of three
       variants — `th` in Gallium (T at 16, H at 19) and `he` in Enthium — and cross-hand is
       exactly the case Chordal Hold does *not* guard. A morph there turns "The" after a pause
       into `⌘H`.
- **Caps Word is armed by double-tapping Shift** (`DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD`). This
  works because 37 is a plain `KC_LSFT`, which is what `process_caps_word.c` requires — it
  accepts only `KC_LSFT` and `OSM(MOD_LSFT)`. `BOTH_SHIFTS` is not usable: it tests
  `mods == MOD_MASK_SHIFT`, and Luz has a single Shift key.
- **Overlay layers need no Shift of their own.** SYMBOLS is held with the right thumb (39), so
  the left thumb reaches 37 freely and the plain Shift covers the whole symbol layer for both
  hands. EXTEND is held with the *same* thumb as 37, so its Shift is position 16 — the
  Select-mode trigger, which is all EXTEND needs.
- **Modifiers live on BASE and are held *before* switching layers.** No overlay carries a
  modifier of its own. This is not a limitation to work around — it is already the required
  pattern for Alt and the bottom-row trio, since on SYMBOLS those positions are occupied by
  digits and symbols. Holding a morph and *then* pressing the layer thumb works because
  `get_chordal_hold_default` returns true whenever either key is `'*'`, and the thumbs are, so
  the mod-tap resolves as a hold; `is_flow_tap_key()` also returns false while Ctrl/GUI/Alt is
  held, so the layer-tap underneath resolves normally. One rule instead of two.

### The layer-scoped mod latch

The bullet above has a cost: the modifier must stay physically held for as long as the chord
lasts, which pins a finger. On SYMBOLS that bites, because the mods sit on the bottom row and
the digits on the same hand's ring, middle and index — so holding left `Ctrl` at 28 while
pressing `5` at 27 asks two adjacent fingers of one hand to do different jobs at once. And
these are the chords you repeat rather than perform once: `Ctrl`/`Alt`/`Cmd` + a digit, over
and over, keying figures into a spreadsheet.

So Luz reads the modifier release differently while SYMBOLS is up:

> **While SYMBOLS is active, releasing a held modifier latches it for the life of the layer.**
> It is released when the layer is.

```
hold Alt          Alt registered
hold SYMBOLS
release Alt       Alt STILL registered   ← latched
  … type on SYMBOLS with no finger pinned on Alt …
release SYMBOLS   Alt released
```

The gesture is **timing-independent** — not a tap, a double-tap or a one-shot, so there is no
term to learn and no window to miss. It is the ordinary hold, read differently because a layer
is up. Three limits are part of the rule:

1. **SYMBOLS only.** EXTEND is a layer of whole commands whose `SK_*` keys build their own
   chords; a latched modifier there would corrupt them. ADJUST inherits the latch because it
   *is* the SYMBOLS thumb plus the EXTEND thumb — releasing the SYMBOLS thumb ends both the
   layer and the latch, which is the same rule rather than an exception to it.
2. **Shift never latches.** Shift is the one modifier used *inside* ordinary typing on this
   layer — every `SY_*` symbol's shifted partner is reached with it — so a latched Shift would
   turn the whole symbol field over to its shifted face until the layer ended.
3. **Layer Lock drops the latch.** `QK_LLCK` turns the chord into a mode; a modifier held
   across that boundary would outlive every cue that it is down. Locking is the explicit
   "I am staying here" gesture, so it is the right place to let the modifiers go.

The rule is safe to state positionally because **no Luz layer above BASE places a mod-tap
anywhere**: a mod-tap release seen while SYMBOLS is up always belongs to a key pressed on BASE.
An implementation on other firmware should reproduce the rule, not the mechanism — there is no
direct ZMK equivalent (a `&sl`-style sticky behaviour is consumed by the next key, where this
persists for the layer).

### Shared (the contract) → `luz/mods.h`

- **Chordal Hold handedness array** (`chordal_hold_layout`) — purely positional (`L`/`R`/`*`,
  thumbs exempt), so it is defined once here and identical for every variant.
- **The Cmd/Ctrl morph** — `LUZ_MORPH_KEY(keycode, record, key, mod)`, a macro
  that expands inside `process_record_user` (same idiom as `SYM_MODTAP_SHIFT`): on a non-macOS
  platform it registers Ctrl for the held index morph keys. The per-layout `morph_l`/`morph_r`
  snapshots stay in `keymap.c` because they wrap that layout's index letter.
- **The layer-scoped mod latch** — [`keyboards/6x3_3/luz/mod_latch.h`](keyboards/6x3_3/luz/mod_latch.h),
  pulled in by `mods.h` because it is part of the mod contract rather than a variant feature. A
  `keymap.c` wires it with two calls: `luz_mod_latch_process()` in `process_record_user` (after
  the morph) and `luz_mod_latch_layer_state()` in `layer_state_set_user` (after the tri-layer
  update). The Layer Lock hook is defined in the header, since it is identical everywhere.
- **Tap-hold tuning** (in the shared [`luz/config.h`](keyboards/6x3_3/luz/config.h), part of the contract): `TAPPING_TERM 240`,
  `CHORDAL_HOLD`, `PERMISSIVE_HOLD`, `FLOW_TAP_TERM 150`. Chordal Hold's opposite-hands rule
  prevents same-hand roll misfires; Flow Tap suppresses holds during fast typing bursts.

### Per-layout

Only the letters under the mods — i.e. nothing but BASE. A variant must place its alphas so the
mod *positions* above still hold (index morph on the index home keys; the Alt/GUI/Ctrl trio on
the bottom row). Enthium mirrors the hands, but because the scheme is itself mirror-symmetric,
every letter keeps the same modifier under the opposite hand.

> [!NOTE]
> **Open tension, surfaced by Luz for Colemak-DH.** The rule above ("a variant must place its
> alphas so the mod positions still hold") assumes the variant author controls the alphas. A
> variant adopting a *published* layout does not: Colemak Mod-DH's bottom row is fixed at
> `z x c d v` / `k h , . /`, which puts ~13.3% of English letters under a bottom-row mod-tap
> against ~6.8% for Gallium East — `H` alone carries almost the whole Gallium load. The natural
> mitigation is a longer per-variant `TAPPING_TERM`, which the tap-hold tuning contract above
> currently forbids. This is recorded, not resolved; see
> [`luz_for_colemak_dh/README.md`](keyboards/6x3_3/keymaps/luz_for_colemak_dh/README.md).

---

## The thumb cluster — *deliberately not a convention*

Luz imposes **nothing** on the thumb cluster beyond what other conventions already require:
the layer model pins the two inner thumbs (EXTEND on the left inner, SYMBOLS layer-tap on the
right inner), the mod system puts the plain Shift on 37, and Space sits on 40. What a variant
puts *under* a pinned hold is free. Thumb 37 is a plain `KC_LSFT` in every variant, so nothing
sits under it at all. Thumb 38 is
the EXTEND hold: `MO(EXTEND)` in Gallium and Colemak-DH, and `LT(EXTEND, KC_R)` in Enthium, which
gives `R` the best thumb key on the board. That tap costs Enthium one thing worth knowing:
`is_flow_tap_key()` matches on the *tap* keycode, and `KC_R` is an alpha, so Flow Tap engages and
EXTEND cannot be entered within `FLOW_TAP_TERM` of a keystroke. `LT(SYMBOLS, KC_ENT)` is exempt
because `KC_ENT` is not a flow-tap key.

**Positions 36 and 41 are blank in every variant.** Everything that used to live there is
duplicated elsewhere — `Esc` on EXTEND at position 12, `Enter` as the tap of `LT(SYMBOLS)` at 39 —
so nothing unique is lost, and a 40-key build is available whenever a variant wants one.

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

1. **Select — hold.** Hold `Sel` (a plain Shift on EXTEND's home row) and every cursor motion
   extends a selection instead of moving; release to stop.
2. **Delete — hold.** Hold `Dl⊙` for the `EXTEND_DEL` sub-layer. Vim-like operator grammar:
   **row = granularity** (line / char / word), each deletion sitting on the motion it consumes.
   EXTEND's motions and clipboard stay live underneath via transparency.
3. **Tabs — hold.** Hold the tab trigger for `EXTEND_TABS`: browser tab management on the
   same inverted-T — index column = tab lifecycle (new / close / reopen), home-row arms = switch,
   plus history back/forward.

Rules: all three are **hold-only** (momentary, never latched — a destructive or contextual mode
can't be left on by accident); they are used **one at a time** — the Delete sub-layer blocks the
Select and Tabs keys.

### Suggested, not imposed

What *fills* the cluster — the `SK_*` semantic keys (OS-aware clipboard, line/word motion,
deletions, tab actions) — is a **suggested default set, not a requirement**. A variant may swap
them, and every **free/empty EXTEND position is open for per-layout functions**. Luz fixes the
cluster's geometry and the sub-mode mechanics; what rides on top is the layout's to choose.

---

## Status

- [x] **Layer model** — enum, naming, activation, structural rules.
- [x] **Symbol set** — the `SY_*` vocabulary and its related-shift pairings; placement
  per-layout under shared principles.
- [x] **Compose** — chord-armed dead-key/diacritic system on thumbs 37+40 (Shift first); the
  sole Luz combo, and shared end to end — positions, operands and behaviour alike.
- [x] **Mod system** — plain Shift on thumb 37, Cmd/Ctrl morph mirrored at 17/18, bottom-row
  Alt/GUI/Ctrl, and the tap-hold tuning that arbitrates all of them.
- [x] **Navigation cluster** — modifier-free inverted-T + three reuse-the-cluster sub-modes
  (Select / Delete / Tabs — all holds); fills are suggested, not imposed.
- [x] **Thumb cluster** — *mostly* pinned now: 38/39/40 by the layer model and Space, 37 by the
  mod system (and, with Compose back on 37+40, by Compose too), 36/41 blank by rule. What
  remains free is only what a variant puts *under* the EXTEND hold on 38 — `R` in Enthium.

### What kind of authority this document has

**This document is the contract; the code is one implementation of it.** Some conventions
happen to be backed by shared headers in this repository (`luz/layers.h`, `luz/symbols.h`,
`luz/mods.h`, `luz/compose.h`) — that is convenient for the QMK variants and it makes drift
between *them* harder, but it is not what makes a convention binding. A Luz variant written
for ZMK, Kanata or anything else compiles none of that code and is no less a variant for it.

So where this spec and the C disagree, the spec is wrong and should be corrected — not the
other way round.

Four tap-hold settings are deliberately left unresolved; the arguments for and against each
are kept in [`TUNING.md`](./TUNING.md) so they are not re-derived every time one comes up. And where a rule is stated in QMK vocabulary, that is shorthand for a
behaviour, not a requirement to use QMK.
