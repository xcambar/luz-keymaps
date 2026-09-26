# 1. The frame

BASE is the only layer a variant writes. Its **frame**, the positions that carry something
other than the layout's letters, is fixed by Luz, so every variant has the same thumbs,
modifiers and envelope in the same places.

```
┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
│  · │    │    │    │    │    │       │    │    │    │    │    │  · │
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│ Tab│    │    │    │    │ ⌘  │       │ ⌘  │    │    │    │    │ Bsp│
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│  · │    │ ⌥  │ ◆  │ ⌃  │    │       │    │ ⌃  │ ◆  │ ⌥  │    │  · │
└────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
                  ┌────┬────┬────┐ ┌────┬────┬────┐
                  │    │ ⇧  │EXT │ │SYM │ ␣  │    │
                  └────┴────┴────┘ └────┴────┴────┘
  ⌘ Cmd/Ctrl morph   ◆ GUI   ⌥ Alt   ⌃ Ctrl   (hold; the tap is the variant's letter)
  EXT = EXTEND (hold)   SYM = SYMBOLS (hold) / Enter (tap)   · = free for the variant
```

![BASE — the Luz frame](../keyboards/6x3_3/luz/keymap_drawer/BASE.svg)

## Rules

1. **Thumbs.** A variant MUST place:
   - **37:** a plain Shift (not a mod-tap: nothing is typed when it is tapped alone);
   - **40:** Space;
   - **38:** the EXTEND layer, active while held;
   - **39:** the SYMBOLS layer while held, Enter when tapped.

   Positions **36 and 41 MUST be blank**. A variant MAY give 38 a tap (it then becomes a
   layer-tap). This is the only thing it may put on a thumb.
2. **Modifiers.** The mod-taps on 17/18 and 26–28 / 31–33 are fixed; see
   [Modifiers](mods.md). Their taps are the variant's letters or symbols.
3. **Home row.** The home row keys 13–16 and 19–22 MUST NOT be dual-function (no mod-taps,
   no layer-taps).
4. **Envelope.** Tab and Backspace MUST sit on the outer columns, Tab on the left and
   Backspace on the right, either on the home row (12 / 23) or the bottom row (24 / 35).
   The pair moves together.
5. Every other position on BASE is the variant's: letters, privileged symbols
   ([Symbols](symbols.md#placement)), or blank.

*Rationale: thumbs.* Luz pins the thumbs only where other conventions need them. The layer
model needs the two inner thumbs, the mod system needs Shift on 37, and Space sits on 40.
What a variant puts *under* a pinned hold is free: Enthium gives 38 an `R` tap, which puts
`R` on the best thumb key on the board. The price is worth knowing. A tap on a letter
makes the key subject to fast-typing protection ([Flow Tap](mods.md#tap-hold-arbitration)),
so EXTEND cannot be entered within 150 ms of a keystroke. SYMBOLS' Enter tap is exempt,
because Enter is not a typing key.

*Rationale: blank outer thumbs.* Everything that used to live on 36 and 41 is reachable
elsewhere: Esc on EXTEND at 12, and Enter as the tap of 39. Nothing unique is lost, and a
40-key keyboard can run any variant.

## Reference implementation (QMK)

The frame is written out in each variant's `keymaps[BASE]`. Letters come from the variant's
`layouts/*.h` position macros (`_00_` … `_41_`); the frame keys are written literally:
`KC_TAB`, `KC_BSPC`, `KC_LSFT`, `MO(EXTEND)` (or `LT(EXTEND, …)`), `LT(SYMBOLS, KC_ENT)`,
`KC_SPC`, `KC_NO`.
