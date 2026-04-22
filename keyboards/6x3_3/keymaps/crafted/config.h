// Copyright 2023 Kael Soares Augusto (@Dwctor)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Callum style mods - faster, more responsive home row mods
#undef TAPPING_TERM
#define TAPPING_TERM 200

// Callum style: IGNORE_MOD_TAP_INTERRUPT is now default behavior in QMK
// No PERMISSIVE_HOLD - this is the key difference from aggressive settings

// Per-key hold on other key press for layers only (not for home row mods)
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// Callum-style oneshot mod timeout (ms) - auto-cancel queued mods after delay
#define ONESHOT_MOD_TIMEOUT 3000

// Retro tapping - tap layer key when released without pressing other keys
#define RETRO_TAPPING
#define RETRO_TAPPING_PER_KEY

// Tapping force hold - prevents accidental taps when rolling
#define TAPPING_FORCE_HOLD_PER_KEY

// Combos always reference layer 0 keycodes, so they work regardless of active base layer
#define COMBO_ONLY_FROM_LAYER 0
