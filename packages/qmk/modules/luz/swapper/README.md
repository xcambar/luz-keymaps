# luz/swapper

**Cmd-Tab on one key**, after Callum Oakley's swapper:
1. The first tap of `CM_SWIN` holds the modifier and taps Tab.
2. Further taps only tap Tab.
3. The modifier stays held until you press or release **any other key**, so the window
   list stays open while you pick.

Shift doesn't end it: hold Shift to go backwards.

```
CM_SWIN, CM_SWIN, a   ->   Cmd down, Tab, Tab, Cmd up, a
```

| Keycode | Alias |
|---|---|
| `COMMUNITY_MODULE_SWAPPER` | `CM_SWIN` |

| `config.h` | Default |
|---|---|
| `SWAPPER_MOD` | `KC_LGUI` (use `KC_LALT` for Alt-Tab) |
| `SWAPPER_KEY` | `KC_TAB` |

To change which keys keep the swapper alive (default: the Shift keys), override
`bool swapper_is_ignored_key(uint16_t keycode)`.

Unlike QMK's bundled `qmk/super_alt_tab`, it has no timeout: the next key ends it.
