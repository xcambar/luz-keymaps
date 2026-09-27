# 3. Modifiers

Modifiers are placed by **position, not by letter**: every variant carries the same
modifier on the same key, so shortcuts are the same muscle memory in every layout. Only
the letter under each modifier changes, and that is just BASE.

## Placement

1. **Shift** is a plain modifier on thumb **37**, the same key for both hands.
2. **Bottom-row mod-taps**, mirrored Alt–GUI–Ctrl from the outer column inwards:

   | Left | 26 | 27 | 28 |  | Right | 31 | 32 | 33 |
   |---|---|---|---|---|---|---|---|---|
   | hold | Alt | GUI | Ctrl |  | hold | Ctrl | GUI | Alt |

   The right-hand Alt (33) is the right Alt, i.e. AltGr on layouts that have one, which
   keeps the host layout's AltGr characters reachable.
3. **The Cmd/Ctrl morph** on the inner index column, **17** (left) and **18** (right):
   mod-taps whose hold is **GUI when the host OS is macOS and Ctrl otherwise**, so ⌘C and
   Ctrl+C are the same chord. A left morph holds a left modifier, a right morph a right
   one.
4. **The home row carries no modifiers** ([frame](frame.md) rule 3).
5. **Modifiers live on BASE.** No overlay layer places a modifier or a mod-tap. To combine a
   modifier with an overlay key, hold the modifier first, then the layer. This is already
   how Alt and the bottom row must work, since on SYMBOLS those positions carry digits and
   symbols.

*Rationale: Shift on a thumb.* The modifier held most often costs no tap-hold arbitration
at all, and needs no mirrored twin. SYMBOLS is held with the right thumb, so the left thumb
reaches Shift freely and one Shift covers the whole symbol layer. EXTEND is held with the
same thumb as Shift, so it has its own Shift at 16: the Select trigger, which is all
EXTEND needs.

*Rationale: the morph on 17/18.*
1. The inner index column is the lightest pair in every layout (≈0.8–4.4% of English
   letters), because layouts put their least frequent letters there.
2. More sharply, the index home pair spans a common **cross-hand bigram** in two of three
   original variants (`th` in Gallium, `he` in Enthium). Cross-hand is exactly the case the
   opposite-hands rule below does *not* guard: a morph there turns "The" after a pause
   into ⌘H.

## Tap-hold arbitration

Every mod-tap and layer-tap is resolved by the same rules. These values are part of the
contract:

| Rule | Value / behaviour |
|---|---|
| **Tapping term** | **240 ms**: a key held past it, with nothing else pressed, is a hold |
| **Opposite hands** | a mod-tap still undecided when a key **on the same hand** is pressed resolves as a **tap** (typing a roll). Thumbs are exempt: they count as either hand |
| **Permissive hold** | a mod-tap still undecided when another key (other hand, or a thumb) is **pressed and released** resolves as a **hold**, before the tapping term |
| **Fast typing** | a mod-tap whose tap is a typing key (letter, `.`, `,`, `;`, `/`, Space), pressed within **150 ms** of the previous typing key, is a **tap**, even if held |

*Note.* The opposite-hands and permissive rules mean a modifier held first and then a
layer thumb resolves as a hold, because thumbs are exempt. The fast-typing rule does not
apply while Ctrl, GUI or Alt is already held. So "hold the modifier, then the layer" is one
rule, not two. Four further settings are deliberately open, argued in
[Open questions](tuning.md).

## Caps Word

**Double-tapping Shift (37)** turns on Caps Word: letters are capitalised until a key
outside the set below is pressed.

| Keys | Under Caps Word |
|---|---|
| letters | capitalised |
| `-` (the symbol key) | becomes `_`, so `SCREAMING_SNAKE_CASE` survives the underscore |
| digits, Backspace, Delete, `_` | continue, unshifted |
| anything else (Space, punctuation...) | ends Caps Word |

## The layer-scoped mod latch

Rule 5 of [Placement](#placement) has a cost: the modifier must stay physically held for
the whole chord, which pins a finger. On SYMBOLS that bites, because the mods sit on the
bottom row and the digits on the same hand's ring, middle and index. Holding Ctrl at 28
while pressing `5` at 27 asks two adjacent fingers of one hand to do different jobs. And
these are the chords you repeat rather than perform once: Ctrl/Alt/Cmd + a digit, over and
over, keying figures into a spreadsheet.

So Luz reads the release differently while SYMBOLS is up:

> **While SYMBOLS is active, releasing a held modifier latches it for the life of the
> layer.** It is released when the layer is.

```
hold Alt          Alt registered
hold SYMBOLS
release Alt       Alt STILL registered   ← latched
  … Alt + SYMBOLS keys, no finger pinned on Alt …
release SYMBOLS   Alt released
```

The gesture is **timing-independent**: not a tap, a double tap or a one-shot, so there is
no term to learn and no window to miss. It is the ordinary hold, read differently because
a layer is up. Three limits are part of the rule:

1. **SYMBOLS only.** EXTEND is a layer of whole commands that build their own chords, and a
   latched modifier would corrupt them. ADJUST inherits the latch because it *is* the
   SYMBOLS thumb plus the EXTEND thumb: releasing the SYMBOLS thumb ends both the layer and
   the latch. That is the same rule, not an exception to it.
2. **Shift never latches.** Shift is typed *with* on this layer (every symbol's shifted
   partner uses it), so a latched Shift would flip the whole symbol field until the layer
   ended.
3. **Locking SYMBOLS drops the latch.** Locking turns the chord into a mode, and a modifier
   held across that boundary would outlive every cue that it is down.

The morph's modifier (Ctrl or GUI) latches like any other.

*Note.* The rule can be stated positionally because no layer above BASE places a
mod-tap: a modifier released while SYMBOLS is up always belongs to a key pressed on BASE.
On firmware without an equivalent, reproduce the rule, not a mechanism. ZMK's `&sl` sticky
layer is a different gesture: it is consumed by the next key, where this persists for the
layer.

## Per-layout

Only the letters under the mods vary, i.e. nothing but BASE. A variant places its letters so
that the mod *positions* above still hold. Enthium mirrors the hands, but the scheme is
itself mirror-symmetric, so every letter keeps the same modifier under the opposite hand.

*Note: an open tension.* The rule assumes the variant author controls the letters. A variant
adopting a *published* layout does not: Colemak Mod-DH's bottom row is fixed at
`z x c d v` / `k h , . /`, which puts ≈13.3% of English letters under a bottom-row mod-tap,
against ≈6.8% for Gallium East. The natural mitigation, a longer per-variant tapping term,
is what the fixed 240 ms above forbids. This is recorded, not resolved (see
[Luz for Colemak-DH](../keyboards/6x3_3/keymaps/luz_for_colemak_dh/README.md)).

## Reference implementation (QMK)

| Rule | Implementation |
|---|---|
| mod-taps | `LALT_T`… on BASE |
| morph | the [`luz/cmd_ctrl_morph`](../modules/luz/cmd_ctrl_morph/) module; the variant names its keys (`LUZ_MORPH_L/R`) |
| opposite hands | QMK's Chordal Hold, with the handedness map in `luz/mods.h` |
| permissive hold / fast typing | `PERMISSIVE_HOLD`, `FLOW_TAP_TERM 150` in `luz/config.h` |
| Caps Word | `DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD` and `caps_word_press_user` in `luz/luz.h` |
| mod latch | the [`luz/mod_latch`](../modules/luz/mod_latch/) module, `MOD_LATCH_LAYER` = SYMBOLS |

QMK's Caps Word detector accepts only a plain left Shift, which 37 is. Its "both Shifts"
trigger can't work with a single Shift key.
