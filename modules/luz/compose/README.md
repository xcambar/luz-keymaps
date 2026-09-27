# luz/compose

Luz's Compose: **armed, then consumed by the next key**, which picks by letter (so the
mnemonics hold whatever the layout):

| Next key | Result |
|---|---|
| `E` / `A` / `U` / `O` / `N` | the dead acute / grave / diaeresis / circumflex / tilde (the accent lands on the letter after) |
| `C` / `W` | `ç` / `€` |
| `Esc` | cancel |
| a plain modifier (Shift...) | ignored: Compose stays armed, so Shift+`E` then `a` gives `Á` |
| anything else | disarms Compose and does what it normally does |

The module has no keycode. You decide how to arm it, by calling `compose_arm()` (from
`compose.h`). Luz uses a Shift-then-Space combo:

```c
void process_combo_event(uint16_t combo_index, bool pressed) {
    if (combo_index == COMBO_COMPOSE && pressed) compose_arm();
}
```

Requires `luz/dead_keys` and `luz/semantic_keys`. List it **before** them in
`keymap.json`, so it sees the next key first.
