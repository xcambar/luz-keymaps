# luz/mod_latch

**While a chosen layer is up, releasing a held modifier doesn't release it.** It stays
registered for as long as the layer lasts:

```
hold Alt            Alt registered
hold the layer
release Alt         Alt still registered    <- latched
  ... Alt+1, Alt+2, Alt+3, no finger pinned on Alt ...
release the layer   Alt released
```

This is for chords you repeat: a modifier plus digits or symbols on a layer, with the
modifier on the same hand as the layer's keys. It is timing-independent: no tap, double
tap or one-shot window to learn. It is the ordinary hold, read differently because the
layer is up.

```c
// config.h
#define MOD_LATCH_LAYER 2   // the layer that latches (required)
```

Limits, by design:
- **Shift never latches.** You type with it on the layer (shifted symbols), so a latched
  Shift would flip the whole layer.
- **Locking the layer (`QK_LLCK`) drops the latch.** A lock turns the chord into a mode,
  and a modifier should not outlive every cue that it is down.
- **Only mod-taps latch:** plain modifier keys keep their normal behaviour. Don't put
  mod-taps on the latch layer itself; the latch assumes every mod-tap release it sees
  belongs to a key pressed below it.

API (`mod_latch.h`): `mod_latch_take(mods)` hands modifiers you registered by hand to the
latch (`luz/cmd_ctrl_morph` does this). `mod_latch_drop()` releases whatever is latched.

Needs community modules API 1.1.0 (the `layer_state_set` hook).
