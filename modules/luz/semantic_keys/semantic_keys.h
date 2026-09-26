// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Semantic Keys: one keycode per editing intent (undo, copy, word left, delete to line
// end, new tab...), sending the chord that means it on the current host OS (requires
// luz/host_os). Single-chord keys are held while the key is held, so they auto-repeat;
// multi-step ones (e.g. Linux "delete to line end" = Shift+End, Backspace) are tapped.
//
// Shift + SK_COPY sends Cut instead. Navigation chords follow HandsDown's semantic keys.

#pragma once

#include <stdint.h>

// Tap the host's chord (or chord sequence) for `sk` (one of the SK_* keycodes).
void tap_semkey_code(uint16_t sk);
