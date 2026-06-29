# Luz for Gallium

`luz_for_gallium` is my daily-driver keymap: the [Luz](../../../../README.md) conventions on the
**Gallium East** alpha layout, built to be easy to memorize and low on cognitive overhead.

The shared interaction model — layers, mods, symbols, Compose, navigation — is documented
in [the README](../../../../README.md). This page covers what's specific to `luz_for_gallium`.

## Main features

- **Gallium East** base alphanum layout
- **Navigation cluster** with per-character/word/line and forward/backward motions, without modifiers
- **Select and Delete modes** in the navigation cluster
- **Compose key** for diacritics
- **Numpad on the left hand** keeps the right hand on the mouse
- **OS-aware keys**: Cut/Copy/Paste, app/window switching, unified Ctrl/Cmd key — consistent across macOS and Linux
- Symbols reorganized by frequency: opening brackets on the index column, and pairs kept together as much as possible

## The layers

<!-- KEYMAP DRAWER -->
![BASE layer](./keymap_drawer/00_BASE.svg)
![SYMBOLS layer](./keymap_drawer/01_SYMBOLS.svg)
![EXTEND layer](./keymap_drawer/02_EXTEND.svg)
![ADJUST layer](./keymap_drawer/03_ADJUST.svg)
<!-- END KEYMAP DRAWER -->
![Navigation modes](./keymap_drawer/04_MODES.svg)
![Compose & diacritics](./keymap_drawer/05_DIACRITICS.svg)

> [!NOTE]
> A printable PDF lives at [`keymap_drawer/luz_for_gallium.pdf`](./keymap_drawer/luz_for_gallium.pdf).

## Reference tables

> [!NOTE]
> Searchable, greppable text twins of the diagrams above. Auto-generated from
> `keymap_drawer/make_*_page.py`.

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

Type `Shift + Space` while on BASE, then a key.

<!-- BEGIN DIACRITICS TABLE -->

| Key | Produces | Example |
|-----|----------|---------|
| `e` | ´ acute (dead key) | `Shift+Space`, `e`, `e` → é |
| `a` | \` grave (dead key) | `Shift+Space`, `a`, `e` → è |
| `u` | ¨ diaeresis (dead key) | `Shift+Space`, `u`, `e` → ë |
| `o` | ˆ circumflex (dead key) | `Shift+Space`, `o`, `e` → ê |
| `c` | ç | `Shift+Space`, `c` → ç |
| `n` | ñ | `Shift+Space`, `n` → ñ |
| `w` | € (euro) | `Shift+Space`, `w` → € |

Armed from the **base layer** with Shift + Space. Dead keys wait for a base letter, so the same accent works on any vowel; any unlisted key cancels.

<!-- END DIACRITICS TABLE -->

</details>

## Building

```bash
qmk compile -kb kaly/kaly42 -km luz_for_gallium
qmk compile -kb 42keebs/cantor_pro/v3/left -km luz_for_gallium
```
