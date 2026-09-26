# 4. Symbols

Symbols split the same way layers do. The **vocabulary and its behaviour** are shared
verbatim; **placement** is where each variant keeps its character.

## The vocabulary

Luz has sixteen symbol keys. Each types its glyph, and with **Shift** a *related* glyph
rather than the one a US keyboard would give:

| Tap | Shift |  | Tap | Shift |  | Tap | Shift |
|---|---|---|---|---|---|---|---|
| `'` | `"` |  | `@` | `#` |  | `(` | `<` |
| `-` | `_` |  | `` ` `` | `~` |  | `)` | `>` |
| `/` | `\|` |  | `=` | `+` |  | `{` | `[` |
| `,` | `?` |  | `$` | `%` |  | `}` | `]` |
| `.` | `!` |  | `&` | `*` |  | `:` | `;` |
|  |  |  | `\` | `^` |  |  |  |

1. This table is the whole contract: adding or removing a row changes every variant.
2. **A symbol behaves the same wherever it sits**, on any layer and on BASE. Its Shift
   partner is part of the key, not of the layer.
3. **Symbols on mod-taps keep their pairs.** A variant MAY put a symbol on a BASE mod-tap
   (`,` and `.` on 32/33, for instance). Tapped with Shift held, it MUST type the Shift
   partner (`?`, `!`), exactly as the plain symbol key would.
4. The host receives each glyph as the standard US-layout key for it (for `<`: Shift + `,`).
   The host layout MUST therefore be US-compatible for the symbols to come out as written.

*Rationale: the pairs.* Brackets are grouped by kind, so a glyph and its partner are the
same *kind* of bracket (`(`/`<`, `{`/`[`, `}`/`]`). `:`/`;` and `-`/`_` keep together the
two glyphs a standard layout already shares on one key. The rest pair a symbol with a
visually or semantically adjacent one (`=`/`+`, `&`/`*`, `@`/`#`, `` ` ``/`~`). The
pairing is the part of Luz that muscle memory transfers.

## Placement

Placement differs between variants, because each layout frees different positions (and
Enthium mirrors the hands). It is free, but governed by these principles:

1. **Privileged symbols sit on BASE; the rest live on SYMBOLS.** "Privileged" is purely a
   placement choice: the punctuation a variant wants without a layer hold.
2. **Digits on the left hand**, as specified below.
3. **Opening brackets on the index column**, with bracket pairs kept together (open over
   open, close over close).
4. **Symbols arranged by frequency** within the SYMBOLS field.
5. **Cross-layer consistency:** a symbol on both BASE and SYMBOLS sits at the same position
   on each.

## SYMBOLS: the shared left half

```
┌────┬────┬────┬────┬────┬────┐
│    │ ```│  7 │  8 │  9 │    │      ``` types a Markdown code fence
├────┼────┼────┼────┼────┼────┤
│ ▽  │  0 │  1 │  2 │  3 │    │      ▽ = transparent (the envelope's Tab, where BASE has it)
├────┼────┼────┼────┼────┼────┤
│ ▽  │ →⇒ │  4 │  5 │  6 │Lock│      →⇒ types -> (with Shift: =>); Lock keeps SYMBOLS on
└────┴────┴────┴────┴────┴────┘
```

- **Digits** form a numpad under the left hand: `1 2 3` on the home row with `0` beside
  them, `4 5 6` below, `7 8 9` above. This is a calculator's rows with the middle and
  bottom swapped, so the digits typed most often sit on the home row. A locked SYMBOLS
  layer then keys figures with one hand while the other stays on the mouse.
- **``` ** types three backticks. **→⇒** types `->`, or `=>` with Shift held. Both
  ignore any other held modifier.
- The right half is the variant's symbol field, under the principles above. Where BASE has
  the envelope's Backspace or a privileged symbol, SYMBOLS falls through to it.

*Example* (Luz for QWERTY's right half):

```
┌────┬────┬────┬────┬────┬────┐
│ `~ │ {[ │ =+ │ }] │ :; │    │
├────┼────┼────┼────┼────┼────┤
│ \^ │ (< │ @# │ )> │ '" │ ▽  │
├────┼────┼────┼────┼────┼────┤
│ $% │ &* │ ,? │ .! │ /| │ ▽  │
└────┴────┴────┴────┴────┴────┘
```

![SYMBOLS — numbers & symbols](../keyboards/6x3_3/luz/keymap_drawer/SYMBOLS.svg)

## Reference implementation (QMK)

- **Symbol table:** `SYMBOL_TABLE` in `keyboards/6x3_3/luz/symbols.h` is the single source.
  Each row generates an `SY_*` keycode and a pair of key overrides live on all layers.
- **Mod-tap symbols:** mod-tap taps can't ride a key override, so `SYM_MODTAP_SHIFT`
  supplies the partner from `process_record_user`.
- **Macros:** `MD_FENCE` and `ARROW_OP` are in `luz/luz.h`.
