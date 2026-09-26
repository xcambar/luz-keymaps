# The Luz specification

**Luz** is a set of keymap conventions for 42-key split keyboards (3×6 + 3 thumbs per
side). Your alpha layout places the letters; Luz specifies everything else: the layers
and how to reach them, where the modifiers sit, the symbols and their shifted partners,
navigation and editing, accents. It stays identical across layouts, so muscle memory
carries from one layout to another.

This specification is **the** definition of Luz. It is written for people implementing
Luz, on any firmware, and for anyone who wants the exact rules behind the
[README](../README.md)'s tour. Implementations follow it; they do not define it:

- The QMK keymaps in this repository are **reference implementations**
  ([guide](../docs/qmk.md)).
- The [QMK community modules](../modules/luz/) package the parts others may want to reuse.
- The [scenarios](../tests/scenarios/) are **executable conformance tests** (see
  [Conformance](conformance.md)).

Where an implementation and this document disagree, the implementation is wrong, or this
document has a bug, which is then fixed here first.

## Reading this specification

The key words **MUST**, **MUST NOT**, **SHOULD**, **SHOULD NOT** and **MAY** are to be
read as in [RFC 2119](https://www.rfc-editor.org/rfc/rfc2119). Text marked *Rationale* or
*Note* is informative. Everything else in a chapter is normative.

Rules are stated as **behaviour**: what a key does, and what the host receives. When a
rule names a QMK feature or keycode, that is shorthand for the behaviour, not a
requirement to use QMK. Each chapter ends with how the reference implementation does it.

### Positions

Keys are identified by **position**, numbered in reading order: the left half of each
row, then its right half, then the thumbs.

```
┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
│  0 │  1 │  2 │  3 │  4 │  5 │       │  6 │  7 │  8 │  9 │ 10 │ 11 │
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│ 12 │ 13 │ 14 │ 15 │ 16 │ 17 │       │ 18 │ 19 │ 20 │ 21 │ 22 │ 23 │
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│ 24 │ 25 │ 26 │ 27 │ 28 │ 29 │       │ 30 │ 31 │ 32 │ 33 │ 34 │ 35 │
└────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
                  ┌────┬────┬────┐ ┌────┬────┬────┐
                  │ 36 │ 37 │ 38 │ │ 39 │ 40 │ 41 │
                  └────┴────┴────┘ └────┴────┴────┘
```

Columns 0/11, 12/23 and 24/35 are the **outer columns**; 5, 17, 29 and 6, 18, 30 are the
**inner (index) columns**. The **thumbs** are 36–41: 37 and 40 are the *home* thumbs, 38
and 39 the *inner* thumbs, 36 and 41 the *outer* thumbs.

### Terms

| Term | Meaning |
|---|---|
| **tap** / **hold** | a press released before the tapping term (240 ms) / held past it, or resolved as a hold by a chord (see [Modifiers](mods.md#tap-hold-arbitration)) |
| **mod-tap** | a key that types a character when tapped and acts as a modifier when held |
| **layer-tap** | a key that types a character when tapped and activates a layer while held |
| **momentary layer** | a key that activates a layer only while held |
| **transparent** (▽) | a position on an upper layer that does what the layer below does |
| **blocked** (✗) | a position that does nothing, where the layer below would do something |
| **chord** | a modifier held while another key is tapped (⌘C) |
| **host OS** | the keyboard's setting for the operating system it talks to (macOS or Linux); chords that differ between them follow it |

## What a variant may change

A **variant** is one alpha layout dropped into the Luz frame. What varies is deliberately
small and deliberately **enumerated**: a conformance checklist, not a vague licence.

| May vary | Must not vary |
|----------|---------------|
| BASE alpha placement | the layer set, its order, and how each layer is reached |
| which symbols are privileged onto BASE, and where | the symbol *vocabulary* and its shift pairings |
| the SYMBOLS right-hand field | the SYMBOLS left-hand numpad |
| the tap of BASE position 18 (it doubles as the right morph) | Shift plain on 37; the morph mirrored on 17/18 |
| what sits under a pinned hold: the morph's taps, and the tap of the EXTEND key on 38 | mod positions: 37, 17/18, 26–28, 31–33, and the mod latch on SYMBOLS |
| the envelope's positions (Tab, Backspace): 12/23 or 24/35 | Compose on thumbs 37+40, Shift first, and everything it emits |
| EXTEND's fills (the commands are a default) | the EXTEND cluster's geometry and the sub-mode rules |

Everything else (EXTEND, EXTEND_DEL, EXTEND_TABS, ADJUST, the SYMBOLS left half) is
identical in every variant. A variant that stays within the left column is a Luz variant,
whatever firmware it is written for.

## Chapters

1. [The frame](frame.md): BASE's fixed structure, the envelope, the thumbs.
2. [Layers](layers.md): the six layers, how each is reached, and the shared ones, position
   by position.
3. [Modifiers](mods.md): placement, the Cmd/Ctrl morph, Caps Word, the mod latch, tap-hold
   arbitration.
4. [Symbols](symbols.md): the vocabulary, its shift pairs, and the placement principles.
5. [Compose](compose.md): accents and special characters.
6. [Navigation and editing](navigation.md): the EXTEND cluster, its sub-modes and its
   commands.
7. [Conformance](conformance.md): the checklist, and the executable scenarios.

Not normative: [open tap-hold questions](tuning.md), the arguments behind the four
settings deliberately left open.

## Status

Luz 3 describes the conventions the reference implementations ship as of v3.0.0 (see the
[changelog](../CHANGELOG.md)). All six areas are settled: layers, symbols, Compose,
modifiers, navigation, thumbs. The one recorded tension, published layouts with heavy
bottom-row mod load, is noted in [Modifiers](mods.md#per-layout).
