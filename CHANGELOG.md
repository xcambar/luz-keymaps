# Changelog

Each `## v…` section below is published as the body of the matching GitHub release: when a
`v*` tag is pushed, CI extracts that section (heading → release title, text → release notes)
and attaches the built firmware. Newest release on top.

## Unreleased

**Luz is documented first.** The specification now stands on its own, independent of any
firmware, and the QMK keymaps are its reference implementations. Nothing changes on the
keyboard.

- **[The specification](./spec/README.md)** replaces `LUZ.spec.md`. It is split into chapters
  (frame, layers, modifiers, symbols, Compose, navigation, conformance) and written as
  behaviour, with each shared layer given position by position. `TUNING.md` is now
  [its open-questions chapter](./spec/tuning.md).
- **[QMK community modules](./modules/luz/README.md):** host_os, semantic_keys, dead_keys,
  compose, cmd_ctrl_morph, mod_latch and swapper, reusable in any keymap. The variants now
  load them through `keymap.json`.
- **[keyspec](./packages/keyspec/README.md):** the behavioural test runner, extracted as a
  standalone, firmware-agnostic tool.
- **[Conformance scenarios](./spec/conformance.md):** a shared suite every variant passes,
  plus per-variant files, run in CI on both boards.
- Local builds need `qmk config user.overlay_dir=<this repo>` so the QMK CLI finds the
  modules ([QMK guide](./docs/qmk.md#building)).

## v3.0.0 — Serpentine

**`crafted` is now Luz.** What was one keymap is a *framework*: a set of shared conventions —
layers, mods, symbols, Compose, navigation — that any alpha layout drops into. It ships as four
variants, and adding a fifth is a day's work.

> [!WARNING]
> **Breaking: the keymaps were renamed.** `-km crafted` no longer exists; use
> `-km luz_for_gallium`. (`0a323fb`)

### The framework

- [`LUZ.spec.md`](./spec/README.md) — the contract, opening with a conformance checklist of what may
  and must not vary, so you can judge a variant without reading its code. (`e03af29`, `50a51d7`)
- Shared headers under `keyboards/6x3_3/luz/` — one source of truth for the layer model, the
  symbol set, the mod system and Compose. (`f1fcb18`, `17c951a`, `c0a1040`, `fa01dfe`)

### New variants

- **Luz for Enthium** — Enthium, hands mirrored; `R` rides the navigation thumb. (`333a034`, `4bb2b25`, `73e4d14`)
- **Luz for Colemak-DH** — the most widely adopted alternative layout, added as a test of the contract. (`69c2f9e`)
- **Luz for QWERTY** — no new alphas to learn. Ported in three lines of code. (`77858b0`)

### Mods

- **Shift is a plain thumb key** — not a dual-function key at all, so the modifier you hold most
  has no timing to get wrong, and one key serves both hands. Double-tap for Caps Word. (`b2fe06b`)
- **The Cmd/Ctrl key moved to the inner index pair**, off the home row and off the cross-hand
  bigram it used to straddle. The home row now carries no dual-function keys. (`b2fe06b`)
- **Mods latch on the symbols layer** — hold Ctrl, enter the layer, let go: it stays until you
  leave, so no finger is pinned on a modifier while the rest of that hand works the numpad.
  Shift is the exception. (`cf06afb`)

### Compose

- **Back on Shift+Space**, and now identical on every variant — positions, chord and output
  alike. Shift must be pressed first. (`79279cb`)
- **`N` arms a tilde dead key** instead of printing `ñ`, so `ã` and `õ` cost what `ñ` costs. (`0afe648`)

### Symbols

- One `SYMBOL_TABLE` generates the keycodes and their shift overrides; the old per-layout symbol
  feature flags are gone. (`1f4e3bc`, `6f87c3c`, `83a72de`, `26619a1`)

### Navigation

- **Select is hold-only**, matching Delete and Tabs. (`b2fa947`)

### Firmware

- Split-transport watchdog, fixing the dead slave half after a macOS sleep/wake. (`fcfe5d9`, `632ae3b`)

### Removed

- The runtime-toggled secondary base layout (`1c50057`), the alternative alpha layouts
  (`24739fd`), and Weak Corners (`c3294c5`).

### Docs & diagrams

- **Shared frame diagrams** showing what every variant has in common, independent of any one
  layout. (`ea7b02e`, `79279cb`, `21cc986`)
- A guide for contributing variants. (`23054b3`)
- Four tap-hold settings recorded as deliberately open, with the arguments for each, in
  [`TUNING.md`](./spec/tuning.md). (`a875e90`)
- Corrections to documentation the mod rework left stale — including a Shift drawn on the wrong
  key in a published diagram. (`939ee00`, `31bcbb3`, `c17fc63`)

**Full changelog:** https://github.com/xcambar/luz-keymaps/compare/v2.0.0...v3.0.0

## v2.0.0 — Jasper

Second release of the **`crafted`** keymap. Two split_3x6_3 keyboards — **Kaly42** (STM32) and **Cantor Pro v3** (RP2040) — sharing the `crafted` and `zen` keymaps.

### Caps Word
- Now armed by **double-tapping Left Shift** (QMK-native), replacing the dedicated toggle key. `_` and `-` keep a `SCREAMING_SNAKE_CASE` run together.

### Navigation & Editing layer
- **New Tabs sub-mode** — browser tab management on the cursor cluster: new / close / switch tabs, reopen, and history back/forward — all OS-aware.
- The three sub-modes — **Delete · Tabs · Select** — are now grouped together on the home row.
- Removed the momentary **⌘/Ctrl** key, and trimmed a stray inherited key.

### Numbers & Symbols layer
- **Numpad reordered** — the digits now run **`0 1 2 3` straight across the home row** (`4 5 6` below, `7 8 9` above), with `0` on the ring finger, instead of the old calculator order with `0` off on the inner column.
- **Layer Lock relocated** off the home row (mirroring the nav layer) to avoid accidental locks.

### Thumbs
- Right inner thumb is now **tap = Enter, hold = SYMBOLS**, cutting thumb travel.
- Streamlined the right-pinky Backspace / Delete key across layers.

### Diagrams & docs
- Layers **renamed and numbered** in the diagrams: `0 · Base`, `1 · Numbers & Symbols`, `2 · Navigation & Editing`, `3 · System & Function`; keycap layer-switch hints now show the destination layer number.
- New **reference pages** rendered as SVG: **Navigation modes** and **Compose & diacritics**.
- README gains **searchable text tables** (auto-generated from the diagram scripts), plus clearer descriptions of every navigation function.

**Full changelog:** https://github.com/xcambar/qmk_userspace/compare/v1.0.0...v2.0.0

## v1.0.0

First tagged release of Xavier's QMK userspace — two **split_3x6_3** keyboards (Kaly42 · STM32 and Cantor Pro v3 · RP2040) sharing two keymaps.

### `crafted` — the daily driver
- **Gallium East** base layout (configurable), with an optional secondary base toggled at runtime
- **Navigation cluster** — per-character / word / line motions, forward & backward, without modifiers
- **Select and Delete modes** layered onto the navigation cluster
- **Compose key** for diacritics
- **Left-hand numpad** so the right hand can stay on the mouse
- **Frequency-ordered symbols** — opening brackets on the index column, pairs kept together
- **OS-aware keys** — Cut/Copy/Paste, app/window switching, and a unified Ctrl/Cmd, consistent across macOS & Linux

### `zen`
- Minimal 3-layer QWERTY fallback, no custom features
