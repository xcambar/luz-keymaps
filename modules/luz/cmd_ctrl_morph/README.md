# luz/cmd_ctrl_morph

The shortcut modifier follows the host OS: chosen **GUI mod-taps hold GUI on macOS and
Ctrl everywhere else** (read from `luz/host_os`). One physical chord is ⌘C on a Mac and
Ctrl+C on Linux. Only the hold changes: the tap, and the tap-or-hold decision, are QMK's
as usual. A left GUI mod-tap becomes left Ctrl, a right one right Ctrl.

Choose the keys in your keymap:

```c
bool is_cmd_ctrl_morph_key(uint16_t keycode) {
    return keycode == LGUI_T(KC_G) || keycode == RGUI_T(KC_H);
}
```

Only these keys morph; other GUI mod-taps stay GUI.

With `luz/mod_latch` in the build (listed **after** this module), a morphed Ctrl latches
like any other modifier. Without it, the weak `mod_latch_take()` fallback does nothing.

Requires `luz/host_os`.
