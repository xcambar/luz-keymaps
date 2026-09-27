// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Layer-scoped Mod Latch: while MOD_LATCH_LAYER is active, releasing a held mod-tap's
// modifier does not release it. The modifier stays registered for as long as the layer
// lasts, and is released with it:
//
//     hold Alt            Alt registered
//     hold the layer
//     release Alt         Alt still registered   <- latched
//       ... Alt + layer keys, no finger pinned on Alt ...
//     release the layer   Alt released
//
// Timing-independent: an ordinary hold, read differently because the layer is up. Limits:
//   - Shift never latches (it is typed with, not chorded);
//   - locking the layer (QK_LLCK) drops the latch: a lock turns a chord into a mode;
//   - only mod-taps pressed on a layer below it latch; don't put mod-taps on the layer.
//
// Configure in config.h:  #define MOD_LATCH_LAYER <layer index>   (required)

#pragma once

#include <stdbool.h>
#include <stdint.h>

// Offer `mods` (a MOD_BIT mask, already registered) to the latch. Returns true if the latch
// took ownership, in which case the caller must not unregister them. For code that
// registers modifiers by hand (e.g. luz/cmd_ctrl_morph).
bool mod_latch_take(uint8_t mods);

// Release everything the latch holds. Idempotent.
void mod_latch_drop(void);
