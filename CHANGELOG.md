# Changelog

Each `## v…` section below is published as the body of the matching GitHub release: when a
`v*` tag is pushed, CI extracts that section (heading → release title, text → release notes)
and attaches the built firmware. Newest release on top.

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
