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

// Caps Word: double-tap Left Shift to turn it on (QMK-native; pos 37 is plain KC_LSFT,
// which the built-in detector requires — it wouldn't work with a mod-tap/Space-Cadet shift).
// NOTE: do NOT also enable CAPS_WORD_INVERT_ON_SHIFT — it strands a real Left Shift on the
// activating double-tap (its handle_shift swallows the Shift release once Caps Word is on),
// leaving Shift stuck down. Without it, Shift simply ends Caps Word (and Space ends it too).
#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
