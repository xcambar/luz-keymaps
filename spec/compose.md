# 5. Compose

Luz writes accented and special characters through **Compose**, not through extra layers or
per-glyph keys. Compose is armed by a chord and consumed by the next key. Writing accents
is a first-class concern of Luz, not an afterthought.

## Arming

**Shift (37), then Space (40)**, pressed together on BASE, arms Compose.

1. **Shift MUST be pressed first.** Space then Shift is not Compose.
2. Arming produces no output: the two keys are consumed.
3. Neither key changes otherwise. Shift alone is Shift, and Space alone is Space.

There is no per-variant half: 37 and 40 are the same keys in every variant.

*Rationale: the operands.* A combo operand must be a key you never type in sequence with
the other operand. Shift satisfies that by not being a typed character at all, which an
alpha pair can never quite do.

*Rationale: Shift first.* It closes the one sequence that could arm Compose by accident.
Rolling out of a Space into the next word's capital (right thumb still down, left reaching
for Shift) would otherwise arm Compose and swallow the space. Pressing Shift first is the
natural gesture, so the constraint costs nothing.

*Note.* Because Shift is a combo operand, a lone Shift press is buffered until it is
released or another key is pressed (at most the combo term). The host sees it slightly
later, and in the same order.

## Consuming

The next key picks the result, then Compose disarms:

| Next key | Result |
|---|---|
| `E` | acute: `é` |
| `A` | grave: `à` |
| `U` | diaeresis: `ü` |
| `O` | circumflex: `ô` |
| `N` | tilde: `ñ ã õ` |
| `C` | `ç` |
| `W` | `€` |
| `Esc` | cancel, nothing typed |

1. **`E A U O N` emit dead keys.** The accent combines with the *next* letter, so Compose,
   `E`, `a` gives `á`. `N` is a dead key rather than a literal `ñ`, so the tilde reaches
   every letter the host allows (`ã`, `õ`) for the same three keystrokes.
2. **`C` and `W` emit the character directly.**
3. **Plain modifiers don't consume Compose.** A held Shift still composes: Compose, Shift+`E`,
   `a` gives `Á`.
4. **Any other key disarms Compose and does what it normally does.**
5. **Keys match by letter, not position.** The mnemonics are chosen by meaning (E for the
   most common acute, C for cedilla), so they are the same in every layout, wherever the
   letters sit.

The long tail Compose omits (`ß`, …) stays reachable through the right Alt (AltGr) on the
bottom-row mod-taps, which produces the host layout's own AltGr characters. Nothing is
locked out; Compose fast-paths the common cases.

## Host requirements

What reaches the host is its own dead-key and character chords, following the host OS
setting:

| | macOS | Linux |
|---|---|---|
| dead keys ´ ` ^ ¨ ~ | ⌥E, ⌥`, ⌥I, ⌥U, ⌥N (US layout) | AltGr + `'` `` ` `` `^` `"` `~` |
| `ç` / `€` | ⌥C / ⌥⇧2 | AltGr+`,` then C / AltGr+`=` then E |

The Linux chords assume a layout whose AltGr layer has dead keys and compose sequences
(EurKEY, or US international with AltGr dead keys).

*Note: history.* Compose briefly lived on positions 5+6, with per-variant operands (`V`+`J`,
`B`+`J`, `X`+`=`). That was a workaround for Shift becoming a mod-tap on a letter, a design
that was explored and **not** adopted: the shipped rework put a plain Shift back on the
thumb. Moving Compose back also retired the last per-variant difference in what was
otherwise shared verbatim.

## Reference implementation (QMK)

- **Combo:** the Shift+Space combo is in `keyboards/6x3_3/luz/compose_combo.h`, with
  `COMBO_MUST_PRESS_IN_ORDER`.
- **What Compose does once armed:** the [`luz/compose`](../modules/luz/compose/) module.
- **Host chords:** the [`luz/dead_keys`](../modules/luz/dead_keys/) and
  [`luz/semantic_keys`](../modules/luz/semantic_keys/) modules, both following
  [`luz/host_os`](../modules/luz/host_os/).
