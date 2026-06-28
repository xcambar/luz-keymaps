# Luz

**Luz is a way to build a complete keyboard layout once and reuse it across different
alpha layouts.** Pick Gallium, Enthium, or anything else for your letters — everything
*around* the letters (how you reach symbols, move the cursor, hold modifiers, type accents)
stays exactly the same. Your muscle memory comes with you when you change layouts.

This page is the friendly tour. For the exact, down-to-the-keycode contract, see
[`LUZ.spec.md`](./LUZ.spec.md).

## The idea

Trying a new alpha layout normally means relearning *everything* — not just where the letters
went, but where you put brackets, how you select a word, which thumb is Shift. That cost is
what keeps most people from ever experimenting.

Luz removes it by drawing a hard line. A Luz keymap has exactly one layout-specific layer: the
**base** layer with the letters on it. Every other layer — navigation, symbols, function keys —
and every behavior — modifiers, Compose, the cursor cluster — is **shared, identical, frozen**.
Swapping from Gallium to Enthium changes your letters and nothing else.

So Luz isn't itself a layout; it's the *frame* a layout drops into. Each concrete keymap is a
**variant**: same Luz frame, different alphas.

| Variant | Letters | Keymap |
|---------|---------|--------|
| **Luz for Gallium** | Gallium East | [`luz_for_gallium`](./keyboards/6x3_3/keymaps/luz_for_gallium/README.md) — the daily driver |
| **Luz for Enthium** | Enthium | [`luz_for_enthium`](./keyboards/6x3_3/keymaps/luz_for_enthium/README.md) — work in progress |

## What you get, everywhere

The same six layers, the same gestures, on every variant:

- **Two thumbs open the two worlds.** One inner thumb holds the **navigation** layer (arrows,
  paging, word/line motion, clipboard, window switching); the other holds the **symbols** layer
  (a numpad on the left hand, a full symbol field on the right).
- **Modifiers by position, not by letter.** The same key is always the same modifier, so
  shortcuts are identical no matter what letter sits there. The main Cmd/Ctrl key even adapts
  per-OS — `⌘C` on macOS, `Ctrl-C` on Linux, same finger — so the whole keymap travels between
  computers without surprises.
- **A symbol vocabulary that pairs by meaning.** Every symbol's Shift gives a *related* symbol
  rather than the QWERTY accident: `(` shifts to `<`, `{` to `[`, `=` to `+`. Learn it once.
- **Compose for the rest of the alphabet.** A thumb chord (Shift + Space) arms Compose; the next
  key adds an accent — `é à ü ô`, `ç`, `ñ`, `€`. Writing beyond plain ASCII is treated as
  first-class, so the keyboard stays friendly to languages other than English. (Anything Compose
  doesn't cover is still on AltGr.)
- **A cursor cluster that doubles as an editor.** The right hand is a modifier-free inverted-T of
  arrows, with distance-from-home meaning distance-moved (character, word, line, page). The same
  cluster gains roles on demand: tap to **select**, hold to **delete** (by character/word/line),
  or hold to drive **browser tabs**.

## What each layout keeps for itself

Luz fixes the frame, but a layout still has room for its own character:

- **The letters**, obviously.
- **The outer thumb keys** — Luz deliberately imposes nothing here.
- **Which symbols get a spot on the base layer** (the rest live on the symbols layer), and how
  the symbol field is arranged.
- **The free keys on the navigation layer**, which you can fill with whatever you like — the
  default editing commands are a *suggestion*, not a requirement.

The discipline is: everything shared is genuinely identical; everything else is genuinely free.
No half-measures, because half-measures are what break muscle memory.

## The principles underneath

- **Consistency beats cleverness.** Same output, same position, on every layer it appears.
- **Position over letter.** Modifiers and triggers are placed by where your finger goes, not by
  which glyph happens to be there — that's what lets the frame outlive any one alpha layout.
- **Restraint.** Exactly one chord (Compose); destructive modes must be *held*, never toggled on
  by accident; the uncomfortable corner keys are left unused on purpose.
- **Openness.** Cross-OS and cross-language behavior is built in, not bolted on.

## Going deeper

- [`LUZ.spec.md`](./LUZ.spec.md) — the exact contract: every layer, keycode, position, and rule.
- The variant READMEs ([luz_for_gallium](./keyboards/6x3_3/keymaps/luz_for_gallium/README.md),
  [luz_for_enthium](./keyboards/6x3_3/keymaps/luz_for_enthium/README.md)) — diagrams and specifics per layout.
- [`keyboards/6x3_3/luz/`](./keyboards/6x3_3/luz/) — the shared code the variants compile against.
