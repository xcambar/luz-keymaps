# Xavier's 3x6+3 Keymaps

![](./header.jpeg)

Keymaps for **3x6+3** keyboards.

Two layouts live in this repository: 

- **`zen`**, a minimal QWERTY fallback (3 layers, no custom features)
- **`crafted`**, is my daily driver, described below.

## The `crafted` keymap

`Crafted` is the keymap I've built to be easy to memorize and bear low cognitive overhead.

### Main features

- **Gallium East** as the (configurable) base alphanum layout
- **Navigation cluster** with per-character/word/line and forward/backward motions, without modifiers.
- **Select and Delete modes** enabled in the navigation cluster
- **Compose key** for diacritics
- **Numpad on the left hand** helps maintain the right hand on the mouse.
- **OS-aware keys**: Cut/Copy/Paste, app/window switching, unified Ctrl/Cmd key, enable consistent access across macOS and Linux
- Symbols reorganized by frequency: opening brackets on the index column, and pairs kept together as much as possible.
- Optional secondary base layer can be toggled at runtime (QWERTY by default). Useful when transitioning between layouts without reflashing.

> [!IMPORTANT]
> By design, the top left and top right keys are never used bevause they're too uncomfortable to reach.

### The layers

<!-- KEYMAP DRAWER -->
![BASE layer](./keyboards/6x3_3/keymaps/crafted/keymap_drawer/00_BASE.svg)
![SYMBOLS layer](./keyboards/6x3_3/keymaps/crafted/keymap_drawer/01_SYMBOLS.svg)
![FAVS layer](./keyboards/6x3_3/keymaps/crafted/keymap_drawer/02_FAVS.svg)
![ADJUST layer](./keyboards/6x3_3/keymaps/crafted/keymap_drawer/03_ADJUST.svg)
<!-- END KEYMAP DRAWER -->

> [!NOTE]
> A printable PDF lives at [`keymap_drawer/crafted.pdf`](./keyboards/6x3_3/keymaps/crafted/keymap_drawer/crafted.pdf).

### Navigation modes

Holding the left inner thumb enters **Navigation** — the cursor and clipboard layer shown
above. Three sub-modes layer on top of it without ever leaving: tap or hold a trigger and the
relevant keys change role. The table below maps each key to what it does in every mode.

| Mode | Engage | Exit |
|------|--------|------|
| **Navigation** (base) | Hold the left inner thumb — or tap **Layer Lock** to keep it on hands-free | Release the thumb (or tap **Layer Lock** again) |
| **Select** · `Sl⊙` | Tap (toggle) | Tap again, **Esc**, or leaving Navigation |
| **Delete** · `Dl⊙` | Hold | Release (hold-only) |
| **Tabs** | Hold | Release (hold-only) |

Select and Delete are mutually exclusive: starting a deletion clears an active selection latch.

| Key | Navigation (base) | Select · `Sl⊙` | Delete · `Dl⊙` | Tabs |
|-----|-------------------|-----------------|-----------------|------|
| `←` | Char left | Select char left | Backspace | Previous tab |
| `→` | Char right | Select char right | Forward-delete | Next tab |
| `↑` | Line up | Select line up | · | New tab |
| `↓` | Line down | Select line down | · | Close tab |
| `◀◀` | Word back | Select word back | Delete word back | Page back |
| `▶▶` | Word forward | Select word forward | Delete word forward | Page forward |
| `⏮` | Line start | Select to line start | Delete to line start | · |
| `⏭` | Line end | Select to line end | Delete to line end | · |
| `PgUp` | Page up | Select page up | · | · |
| `PgDn` | Page down | Select page down | · | · |
| `●` | · | · | · | Reopen tab |

`·` = the key has no special role in that mode (it keeps its Navigation behaviour, or nothing).
Tab actions are OS-aware (Firefox & Chrome, macOS & Linux).

### Compose & diacritics

Tap **Shift + Space** (both inner thumbs together) to arm Compose, then:

| Key | Output | |
|-----|--------|--|
| `e` | ´ acute (dead key) | `Shift+Space`, `e`, `e` → é |
| `a` | \` grave (dead key) | `Shift+Space`, `a`, `e` → è |
| `u` | ¨ diaeresis (dead key) | `Shift+Space`, `u`, `e` → ë |
| `o` | ˆ circumflex (dead key) | `Shift+Space`, `o`, `e` → ê |
| `c` | ç | |
| `n` | ñ | |
| `w` | € | |
| `Esc` | cancel | |

Any other key cancels Compose and types as usual.

### Building

I have run the keymaps on the following MCUs:

| MCU | Example Keyboard | 
|----------|-----|
| STM32 | **Kaly42** (`kaly/kaly42`) |
| RP2040 | **Cantor Pro v3** (`42keebs/cantor_pro/v3/left`) |

```bash
qmk compile -kb kaly/kaly42 -km crafted
qmk compile -kb 42keebs/cantor_pro/v3/left -km crafted
```

With a different base layout:

```bash
XC_LAYOUT=graphite qmk compile -kb kaly/kaly42 -km crafted
```

Build options (`rules.mk` or environment):

- **`XC_LAYOUT`** (default: `gallium_east`) — base layout: `qwerty`, `gallium`, `gallium_east`, `focal`, `graphite`
- **`XC_SECONDARY_LAYOUT`** (default: `qwerty`) — the alternate base layer, toggled from `ADJUST`
- **`XC_WEAK_CORNERS`** (default: `no`) — access corner letters via combos
- **`XC_ALT_BASE_SYMBOLS`** (default: `yes`) — semantic shift pairs on the base layer

All targets at once:

```bash
qmk userspace-compile
```

## Inspiration

- **[HandsDown](https://sites.google.com/alanreiser.com/handsdown)** — semantic, platform-aware editing commands
- **[Gallium](https://github.com/GalileoBlues/Gallium) East** — the alpha layout
- **Pascal Getreuer's QMK work** — Chordal Hold, Caps Word, and the wider tap-hold tuning vocabulary
- **Callum-style oneshot modifiers** — an early influence, since fully replaced by layer-scoped latches and plain momentary mods
- **[keymap-drawer](https://github.com/caksoylar/keymap-drawer)** — the layer diagrams

## License

GPL-2.0-or-later (following QMK licensing)
