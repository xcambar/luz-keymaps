# Luz

**Luz** is a framework of shared conventions and features for alternative keyboard
layouts on the `split_3x6_3` (42-key) form factor. It defines a common interaction
model — layer set, mod placement, navigation, symbols, compose — that stays identical
across layouts, while each layout keeps its own character of alpha key assignments.

Luz ships as layout-specific **variants**:

| Variant            | Layout       | Keymap directory                      |
|--------------------|--------------|----------------------------------------|
| **Luz for Gallium**| Gallium East | `keyboards/6x3_3/keymaps/crafted`      |
| **Luz for Enthium**| Enthium      | `keyboards/6x3_3/keymaps/enthium`      |

A *variant* differs from every other variant in exactly one place: **layer 0 (BASE)**.
Everything above BASE is Luz, and is shared verbatim.

---

## The shared layer model

This is the first and load-bearing Luz convention: a fixed set of six layers with fixed
roles, ordering, and activation. The enum is defined once, in the canonical shared header
[`keyboards/6x3_3/luz/layers.h`](keyboards/6x3_3/luz/layers.h), which is symlinked into
each variant's keymap directory and `#include`d by its `keymap.c`. No variant redefines
it; renaming a layer happens in one file.

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

## Status

- [x] **Layer model** — enum, naming, activation, structural rules (this document).
- [ ] Homerow / bottom-row mods
- [ ] Thumb cluster contract
- [ ] Symbols system
- [ ] Combos (Compose, …)
- [ ] Navigation cluster + sub-mode design

Each convention, as it is defined, is backed by shared code where practical (a shared
header, a shared feature file) and documented here as the authority. Where a convention
cannot yet be shared as code, this document is the contract the variants are kept against.
