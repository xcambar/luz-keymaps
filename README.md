# Luz — keymap conventions for 3x6+3 keyboards

![](./header.jpeg)

**Luz** (Spanish: *luz* /luθ/, "light") completes your favourite alpha layout (Graphite, Focal, Canary... or QWERTY if you will) into a ready-to-use keymap. Since an alpha layout typically only dictates where the letters go, Luz supplies everything else to actually be productive (symbols, numbers, navigation, editing, modifiers, accents) as one consistent, ready-made set of conventions and features.

Luz isn't itself a layout; it's the *framework* a layout drops into. Each concrete keymap is a
**variant**:

| Variant | Letters | Keymap |
|---------|---------|--------|
| **Luz for Gallium** | Gallium East | [`luz_for_gallium`](./keyboards/6x3_3/keymaps/luz_for_gallium/README.md) — the daily driver |
| **Luz for Enthium** | Enthium | [`luz_for_enthium`](./keyboards/6x3_3/keymaps/luz_for_enthium/README.md) — work in progress |

New variants are very welcome- See [Contributing variants](#contributing-variants).

(Another keymap, `zen`, is also in the repository but unrelated to Luz: a minimal QWERTY fallback for early bring-up or recovery. It might be deprecated eventually.)

## What you get, out of the box

- **Bottom-row mods, an OS-aware Cmd/Ctrl, and a thumb Shift.** Mods sit on the bottom row, under the less frequent letters, which reduces misfires. The index home key is a single shortcut modifier that adapts to the OS (`⌘C` on macOS, `Ctrl-C` on Linux). **Shift is a plain, fixed key on the thumb**, present on every layer, so you can hold it to extend a selection while navigating, or shift a character or a motion.

The diagram below is the **Luz for Gallium** variant as an illustration:

![BASE layer](./keyboards/6x3_3/keymaps/luz_for_gallium/keymap_drawer/00_BASE.svg)

- **A rethought symbol vocabulary.** The symbol set is redesigned for mnemonics and frequency, and every symbol's Shift gives a *related* glyph (`(` shifts to `<`, `{` to `[`, `=` to `+`). Learn the logic once and it holds everywhere.
- **Numpad on the left hand.** On the symbols layer the left hand becomes a calculator-style number pad, so you can key in figures one-handed and keep the right hand on the mouse.

![SYMBOLS layer](./keyboards/6x3_3/keymaps/luz_for_gallium/keymap_drawer/01_SYMBOLS.svg)

- **A cursor cluster that doubles as an editor.** The right hand is a modifier-free inverted-T of arrows, with distance-from-home meaning distance-moved (character, word, line, page). **Triggers on the left hand** add extra modes onto that same cluster: one turns the motions into **select**, one into **delete** (by character/word/line), one drives **browser tabs**: the right hand keeps moving, the left hand picks the mode.

![EXTEND layer](./keyboards/6x3_3/keymaps/luz_for_gallium/keymap_drawer/02_EXTEND.svg)
![Navigation modes](./keyboards/6x3_3/keymaps/luz_for_gallium/keymap_drawer/04_MODES.svg)

- **Diacritics through Compose.** A thumb chord (Shift + Space) arms Compose; the next key adds an accent (`é à ü ô`, `ç`, `ñ`) so writing beyond plain ASCII is first-class, not an afterthought. Whatever Compose doesn't cover stays reachable on AltGr, for completeness.

![Compose & diacritics](./keyboards/6x3_3/keymaps/luz_for_gallium/keymap_drawer/05_DIACRITICS.svg)

---

## Room to grow

Luz fills the keys you actually need, but it **deliberately leaves blanks** and empty positions on
several layers. Those are yours: map whatever suits
your workflow onto them, on any layer, without disturbing the shared frame.

---

## Contributing variants

Luz is built to grow, and **new variants are very welcome.** If your favourite alpha layout (Graphite, Focal, Canary, Colemak-DH…) doesn't have one yet, take the existing variants as working examples to copy, and [`LUZ.spec.md`](./LUZ.spec.md) is the contract a variant follows.

Open an issue to discuss, or send a pull request: community-driven variants are exactly how Luz is meant to spread.

---

## Building

I have run these keymaps on:

| MCU | Example keyboard |
|-----|------------------|
| STM32 | **Kaly42** (`kaly/kaly42`) |
| RP2040 | **Cantor Pro v3** (`42keebs/cantor_pro/v3/left`) |

Build a single target, or all of them at once:

```bash
qmk compile -kb 42keebs/cantor_pro/v3/left -km luz_for_gallium
qmk userspace-compile
```

## Going further

- [`LUZ.spec.md`](./LUZ.spec.md) — the exact, implementation-facing contract: every layer, keycode, position, and rule.
- The variant READMEs ([Luz for Gallium](./keyboards/6x3_3/keymaps/luz_for_gallium/README.md), [Luz for Enthium](./keyboards/6x3_3/keymaps/luz_for_enthium/README.md)) — diagrams and per-layout specifics.
- [`keyboards/6x3_3/luz/`](./keyboards/6x3_3/luz/) — the shared code the variants compile against.


## Inspiration

- **[HandsDown](https://sites.google.com/alanreiser.com/handsdown)** — semantic, platform-aware editing commands
- **[Miryoku](https://github.com/manna-harbour/miryoku)** — minimal, consistent layers built on home-row mods
- **[Seniply](https://stevep99.github.io/seniply/)** — the Extend layer: one-handed navigation and editing without leaving home
- **[Anymak](https://github.com/rpnfan/Anymak/)** — a complete keymap designed to sit on top of any alpha layout
- **[Gallium](https://github.com/GalileoBlues/Gallium) East** & **[Enthium](https://github.com/sunaku/enthium)** — the alpha layouts
- **Pascal Getreuer's QMK work** — Chordal Hold, Caps Word, and the wider tap-hold tuning vocabulary
- **Callum-style oneshot modifiers** — an early influence, since fully replaced by layer-scoped latches and plain momentary mods
- **[keymap-drawer](https://github.com/caksoylar/keymap-drawer)** — the layer diagrams

## License

GPL-2.0-or-later (following QMK licensing)
