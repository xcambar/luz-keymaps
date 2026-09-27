# Luz for Gallium

`luz_for_gallium` is my daily-driver keymap: the [Luz](../../../README.md) conventions on the
**Gallium East** alpha layout, built to be easy to memorize and low on cognitive overhead.

The shared interaction model — layers, mods, symbols, Compose, navigation — is documented
in [the README](../../../README.md). This page covers what's specific to `luz_for_gallium`.

## Main features

- **Gallium East** base alphanum layout
- **Navigation cluster** with per-character/word/line and forward/backward motions, without modifiers
- **Select and Delete modes** in the navigation cluster
- **Compose key** for diacritics
- **Digits on the left hand** — `1 2 3` on the home row, `4 5 6` below, `7 8 9` above; lock the layer and figures go in one-handed
- **Mods latch on the symbols layer**: hold Ctrl/Alt/Cmd, enter the layer, let go — the mod stays until you leave, so no finger is pinned on it while you type (Shift excluded; Layer Lock releases)
- **OS-aware keys**: Cut/Copy/Paste, app/window switching, unified Ctrl/Cmd key — consistent across macOS and Linux
- Symbols reorganized by frequency: opening brackets on the index column, and pairs kept together as much as possible

## The layers

<!-- KEYMAP DRAWER -->
![BASE layer](./00_BASE.svg)
![SYMBOLS layer](./01_SYMBOLS.svg)
![EXTEND layer](./02_EXTEND.svg)
![ADJUST layer](./03_ADJUST.svg)
<!-- END KEYMAP DRAWER -->
![Navigation modes](./04_MODES.svg)
![Compose & diacritics](./05_DIACRITICS.svg)

> [!NOTE]
> A printable PDF lives at [`luz_for_gallium.pdf`](./luz_for_gallium.pdf).

## Reference tables

> [!NOTE]
> Searchable, greppable text twins of the diagrams above. Auto-generated from
> `spec/diagrams/make_*_page.py`.

<details>
<summary><strong>Navigation modes</strong></summary>

EXTEND cursor layer + Select / Delete / Tabs sub-modes

<!-- BEGIN NAV TABLE -->

| Key | Navigation (EXTEND layer) | Select (hold Sel) | Delete (hold Dl⊙) | Tabs (hold tab key) |
|-----|------|------|------|------|
| ◀ | Char left | Select char left | Backspace | Previous tab |
| ▶ | Char right | Select char right | Forward-delete | Next tab |
| ▲ | Line up | Select line up | · | New tab |
| ▼ | Line down | Select line down | · | Close tab |
| ◀◀ | Word back | Select word back | Delete word back | Page back |
| ▶▶ | Word forward | Select word forward | Delete word forward | Page forward |
| ⏮ | Line start | Select to line start | Delete to line start | · |
| ⏭ | Line end | Select to line end | Delete to line end | · |
| PgUp | Page up | Select page up | · | · |
| PgDn | Page down | Select page down | · | · |
| ● | · | · | · | Reopen tab |

`·` = the key keeps its Navigation role in that mode. Select and Delete are mutually exclusive. Tab actions are OS-aware (Firefox & Chrome, macOS & Linux).

<!-- END NAV TABLE -->

</details>

<details>
<summary><strong>Compose &amp; diacritics</strong></summary>

Press `Shift` then `Space` (the thumb pair, positions 37+40) while on BASE, then a key. Shift must come first — see [Compose](../../compose.md).

<!-- BEGIN DIACRITICS TABLE -->

| Key | Produces | Example |
|-----|----------|---------|
| `e` | ´ acute (dead key) | `Shift + Space`, `e`, `e` → é |
| `a` | \` grave (dead key) | `Shift + Space`, `a`, `e` → è |
| `u` | ¨ diaeresis (dead key) | `Shift + Space`, `u`, `e` → ë |
| `o` | ˆ circumflex (dead key) | `Shift + Space`, `o`, `e` → ê |
| `n` | ˜ tilde (dead key) | `Shift + Space`, `n`, `n` → ñ |
| `c` | ç | `Shift + Space`, `c` → ç |
| `w` | € (euro) | `Shift + Space`, `w` → € |

Armed from the **base layer** with Shift + Space. Dead keys wait for a base letter, so the same accent works on any base letter; any unlisted key cancels.

<!-- END DIACRITICS TABLE -->

</details>

## Implementations

- **QMK:** [`luz_for_gallium`](../../../packages/qmk/keyboards/6x3_3/keymaps/luz_for_gallium/), with its build commands.

The diagrams and PDFs on this page are built from the `*.yml` files here, from the repository root:

```bash
spec/diagrams/build_pdf.sh gallium
```
