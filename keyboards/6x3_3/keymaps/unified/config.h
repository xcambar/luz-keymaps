// Copyright 2025 Xavier (@xavier)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Tapping behavior (crafted defaults)
#undef TAPPING_TERM
#define TAPPING_TERM 200
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define ONESHOT_TIMEOUT 3000
#define RETRO_TAPPING
#define RETRO_TAPPING_PER_KEY
#define TAPPING_FORCE_HOLD_PER_KEY

// Combos always reference layer 0 (C_BASE) keycodes
#define COMBO_ONLY_FROM_LAYER 0

// Auto Shift (enabled for zen mode, toggled off for crafted mode at runtime)
#define AUTO_SHIFT_TIMEOUT 220
