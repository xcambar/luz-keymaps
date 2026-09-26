// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Dead Keys: keycodes that tap the host's own dead key for an accent (acute, grave,
// circumflex, diaeresis, tilde), so the next letter the host receives gets that accent.
// The chord depends on the host OS (requires luz/host_os):
//   macOS: Option + E / ` / I / U / N          (standard US layout)
//   Linux: AltGr + ' / ` / ^ / " / ~           (EurKEY or US-intl with AltGr dead keys)

#pragma once

#include <stdint.h>

// Tap the host's dead key for `dk` (one of the DK_* keycodes). No-op for any other keycode.
void tap_deadkey_code(uint16_t dk);
