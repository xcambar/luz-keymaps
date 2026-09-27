# 6. Navigation and editing

EXTEND's **right hand is a modifier-free navigation cluster**. Its **left hand holds the
controls** and the launch points for three sub-modes. Luz fixes the cluster's *shape* and
the sub-modes' *behaviour*. The commands that fill them are a default, not a requirement
(see [Fills](#fills)).

## EXTEND

```
┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
│    │    │    │    │    │    │       │PgUp│ L← │ ↑  │ L→ │    │    │
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│ Esc│    │ Dl⊙│ Tb⊙│ Sel│ SWn│       │PgDn│ ←  │ ↓  │ →  │    │ Del│
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│ ▽  │Undo│ Cut│Copy│Pste│Lock│       │    │ W← │    │ W→ │    │    │
└────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
  L←/L→ line begin/end   W←/W→ word left/right   Sel = Shift (Select)
  Dl⊙ / Tb⊙ hold for EXTEND_DEL / EXTEND_TABS    SWn = window switcher
```

![EXTEND — navigation & editing](../keyboards/6x3_3/luz/keymap_drawer/EXTEND.svg)

### The cluster (shape)

1. **Inverted-T arrows** on the right hand (`↑` over `← ↓ →`), **modifier-free**: no mods
   and no dual-function keys on the navigation layer, so motions stay bare cursor keys.
   This keeps them usable in terminals and alongside a mouse.
2. **Magnitude grows away from the home row:** character motion on the home arms, **line**
   motion on the row above (flanking `↑`), **word** motion on the row below, **paging** as a
   vertical pair on the inner column. Distance from home reads as distance moved.
3. **Left hand:**
   - **Esc** on the outer home position;
   - **the three sub-mode triggers** on the home row (14, 15, 16);
   - **the window switcher** on the inner column;
   - **Undo, Cut, Copy, Paste** under the fingers on the bottom row, and **Lock** at 29.
4. **Delete** (forward) on the right outer home position.

### The window switcher

A tap of **SWn** holds GUI and taps Tab; further taps only tap Tab. GUI stays held until
any other key is pressed or released, so the window list stays open while you pick. Shift
doesn't end it: holding Shift goes backwards.

## Sub-modes

Three sub-modes launch from EXTEND's left home row and **reuse the same right-hand
cluster**, putting a role onto a spatial vocabulary you already know:

1. **Select (hold 16).** 16 is a plain Shift: every cursor motion extends a selection
   instead of moving. Release to stop.
2. **Delete (hold 14).** Activates EXTEND_DEL. It follows a vim-like operator grammar:
   **row = granularity** (line / character / word), each deletion sitting on the motion it
   consumes.
3. **Tabs (hold 15).** Activates EXTEND_TABS: browser tab management on the same
   inverted-T. The index column is the tab lifecycle (new / close / reopen), the home-row
   arms switch tabs, and the bottom arms go back and forward in history.

Rules:
1. All three are **hold-only**. A destructive or contextual mode can't be left on by
   accident.
2. **One at a time.** EXTEND_DEL blocks Select, Tabs and Lock. EXTEND_TABS blanks every
   finger key outside its own cluster.
3. On EXTEND_DEL, the rest of EXTEND stays live by transparency: navigate, page or Undo
   without letting go.

```
EXTEND_DEL                                  EXTEND_TABS
┌───┬───┬───┬───┬───┬───┐ ┌───┬───┬───┬───┬───┬───┐   ┌───┬───┬───┬───┬───┬───┐ ┌───┬───┬───┬───┬───┬───┐
│ ▽ │ ▽ │ ▽ │ ▽ │ ▽ │ ▽ │ │ ▽ │DlB│ ▽ │DlE│ ▽ │ ▽ │   │   │   │   │   │   │   │ │   │   │New│   │   │   │
├───┼───┼───┼───┼───┼───┤ ├───┼───┼───┼───┼───┼───┤   ├───┼───┼───┼───┼───┼───┤ ├───┼───┼───┼───┼───┼───┤
│ ▽ │ ▽ │(▽)│ ✗ │ ✗ │ ▽ │ │ ▽ │Bsp│ ▽ │Del│ ▽ │ ▽ │   │   │   │   │(▽)│   │   │ │   │ ◀ │Cls│ ▶ │   │   │
├───┼───┼───┼───┼───┼───┤ ├───┼───┼───┼───┼───┼───┤   ├───┼───┼───┼───┼───┼───┤ ├───┼───┼───┼───┼───┼───┤
│ ▽ │ ▽ │ ▽ │ ▽ │ ▽ │ ✗ │ │ ▽ │DlW│ ▽ │Dl→│ ▽ │ ▽ │   │   │   │   │   │   │   │ │   │ ◅ │Rop│ ▻ │   │   │
└───┴───┴───┴───┴───┴───┘ └───┴───┴───┴───┴───┴───┘   └───┴───┴───┴───┴───┴───┘ └───┴───┴───┴───┴───┴───┘
  DlB/DlE delete to line begin/end   DlW/Dl→ delete word back/forward     ◀ ▶ previous/next tab
  (▽) the held trigger   ✗ blocked                                         ◅ ▻ history back/forward
```

![Navigation modes](../keyboards/6x3_3/keymaps/luz_for_gallium/keymap_drawer/04_MODES.svg)

## Fills

What *fills* the cluster (clipboard, line and word motion, deletions, tab actions) is a
**default set of commands, not a requirement**. A variant MAY swap them, and every blank
EXTEND position is free for per-layout functions. Luz fixes the geometry and the sub-mode
mechanics; what rides on top is the layout's to choose.

The default commands are **semantic**: each is one intent, sent as the chord that means it
on the host OS.

| Command | macOS | Linux |
|---|---|---|
| Undo / Cut / Copy / Paste | ⌘Z / ⌘X / ⌘C / ⌘V | Ctrl+Z / X / C / V |
| Word left / right | ⌥← / ⌥→ | Ctrl+← / → |
| Line begin / end | ⌘← / ⌘→ | Home / End |
| Delete word back / forward | ⌥⌫ / ⌥⌦ | Ctrl+⌫ / Ctrl+Del |
| Delete to line begin / end | ⌘⌫ / Ctrl+K | Shift+Home ⌫ / Shift+End ⌫ |
| New / close / reopen tab | ⌘T / ⌘W / ⌘⇧T | Ctrl+T / W / Shift+T |
| Previous / next tab | ⌘⌥← / ⌘⌥→ | Ctrl+PgUp / PgDn |
| History back / forward | ⌘[ / ⌘] | Alt+← / → |

1. A single-chord command is held while its key is held, so it repeats like the chord would.
2. **Shift + Copy cuts.**

## Reference implementation (QMK)

- **Commands:** the [`luz/semantic_keys`](../modules/luz/semantic_keys/) module (`SK_*`),
  following [`luz/host_os`](../modules/luz/host_os/).
- **Window switcher:** the [`luz/swapper`](../modules/luz/swapper/) module (`SW_WIN`).
- **Sub-layers:** `MO(EXTEND_DEL)` and `MO(EXTEND_TABS)`, with `XXXXXXX` for the blocked
  keys.
