// Copyright 2023 Kael Soares Augusto (@Dwctor)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Tap-hold tuning for bottom-row mod-taps + Chordal Hold
#undef TAPPING_TERM
#define TAPPING_TERM 240

// Chordal Hold: opposite-hands rule for mod-tap keys (QMK Feb 2025)
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD

// Flow Tap: disable hold during fast typing bursts (QMK May 2025)
#define FLOW_TAP_TERM 150

// Combos always reference layer 0 keycodes, so they work regardless of active base layer
#define COMBO_ONLY_FROM_LAYER 0
