// Copyright 2025 Xavier (@xavier)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Tapping term for layer switching (in milliseconds)
// Adjust this if layer switching feels too slow or too fast
#undef TAPPING_TERM
#define TAPPING_TERM 200

// Enable permissive hold for better layer tap behavior
// This makes it easier to quickly hold for layers while typing
#define PERMISSIVE_HOLD

// Quick tap term to allow fast repeated taps
#define QUICK_TAP_TERM 100

// Combo configuration
// Increased timeout for reliable 4-key combo activation
#define COMBO_TERM 150  // Time window for combo activation (ms)

// Use strict timer mode - combo window starts on first key press
// This prevents accidental triggers during normal typing
#define COMBO_STRICT_TIMER

// Auto Shift configuration
// Keys held longer than this threshold will produce shifted characters
#define AUTO_SHIFT_TIMEOUT 220
