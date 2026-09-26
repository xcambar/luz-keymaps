# 2. Layers

Luz's first and load-bearing convention is a fixed set of six layers with fixed roles,
order and activation.

| # | Layer | Role | Reached by | Shared |
|---|---|---|---|---|
| 0 | **BASE** | the layout's letters, in the [frame](frame.md) | default | frame only |
| 1 | **EXTEND** | navigation and editing hub; launches the sub-layers | holding thumb 38 | yes |
| 2 | **SYMBOLS** | numpad (left) and symbol field (right) | holding thumb 39 (tap: Enter) | left half |
| 3 | **EXTEND_DEL** | deletion | holding 14 on EXTEND | yes |
| 4 | **EXTEND_TABS** | browser tabs and history | holding 15 on EXTEND | yes |
| 5 | **ADJUST** | F-keys, media, the host OS setting, bootloader | holding 38 and 39 together | yes |

## Rules

1. **The layer set and its order are fixed.** An implementation MUST provide exactly these
   layers, reached exactly this way. Where a firmware stacks layers, the order MUST be the
   one above: the overlays (EXTEND, SYMBOLS) first, then the sub-layers, then ADJUST, so
   transparency resolves `ADJUST → SYMBOLS → EXTEND → BASE`.
2. **Sub-layers are hold-only.** EXTEND_DEL and EXTEND_TABS MUST be momentary, never
   latched or locked, and are reached from EXTEND only. A destructive layer you must
   actively hold cannot be left on by accident. They are used one at a time (see
   [Navigation](navigation.md#sub-modes)).
3. **ADJUST is a tri-layer.** It is active exactly while both EXTEND and SYMBOLS are, and it
   is the **only** way to reach the bootloader.
4. **EXTEND and SYMBOLS can be locked** by their Lock key (EXTEND 29, SYMBOLS 29), which
   keeps the layer on after its thumb is released; pressing Lock again unlocks it.
5. **Cross-layer consistency.** The same output lives at the same position on every layer
   it appears on, even at the cost of an empty slot elsewhere. A glyph never migrates
   between layers.
   - Shift obeys this by staying out of the overlays: 37 is transparent everywhere above
     BASE, so Shift is on 37 on every layer.
   - EXTEND's second Shift, on 16, is an addition rather than a relocation: it is the
     Select trigger, not a Shift you reach for to capitalise.
6. **The envelope and privileged symbols fall through.** On overlay layers, Tab, Backspace
   and BASE's privileged symbols stay reachable by transparency rather than being declared
   again. The envelope's positions are per variant (12/23 or 24/35), so which positions
   are transparent differs, though the rule does not. Getting this wrong is silent: the
   key is simply missing.
7. **Thumbs on overlays.** Above BASE every thumb is transparent, so Shift, Space and the
   *other* layer key keep working (which is how ADJUST is reached from either overlay).
   The one exception is the key holding the current layer, which is blank on that layer:
   38 on EXTEND and 39 on SYMBOLS.

## ADJUST

```
┌────┬────┬────┬────┬────┬────┐       ┌────┬────┬────┬────┬────┬────┐
│    │ F1 │ F2 │ F3 │ F4 │ F5 │       │ F6 │ F7 │ F8 │ F9 │ F10│    │
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│Boot│ OS⇄│    │    │    │ F11│       │ F12│Mute│Vol+│Brt+│    │    │
├────┼────┼────┼────┼────┼────┤       ├────┼────┼────┼────┼────┼────┤
│    │ OS?│    │    │    │    │       │    │PScr│Vol-│Brt-│    │    │
└────┴────┴────┴────┴────┴────┘       └────┴────┴────┴────┴────┴────┘
```

- **Boot** (12) enters the bootloader.
- **OS⇄** (13) switches the host OS setting (macOS ↔ Linux). **OS?** (25) types its name.
- **Volume and brightness** are vertical pairs, up on the home row and down below.
- **PScr** is Print Screen. On macOS, screenshots stay on ⌘⇧3/4.

Blank positions do nothing. Every thumb is transparent: ADJUST is only up while both
layer thumbs are held.

## Reference implementation (QMK)

- **Layer enum:** `keyboards/6x3_3/luz/layers.h`.
- **Tri-layer:** `update_tri_layer_state` in `luz/luz.h`.
- **Lock:** QMK's Layer Lock (`QK_LLCK`).
- **Host OS setting:** the [`luz/host_os`](../modules/luz/host_os/) module (`SW_OS`,
  `PR_OS`).
