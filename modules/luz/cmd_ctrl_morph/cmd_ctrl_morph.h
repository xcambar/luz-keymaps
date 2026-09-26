// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Cmd/Ctrl Morph: the shortcut modifier follows the host OS. Chosen GUI mod-taps hold GUI
// on macOS and Ctrl everywhere else (requires luz/host_os), so ⌘C and Ctrl+C are the same
// chord. Only the hold changes; the tap and the tap-hold decision are QMK's.
//
// The keymap picks which mod-taps morph:
//
//     bool is_cmd_ctrl_morph_key(uint16_t keycode) {
//         return keycode == LGUI_T(KC_G) || keycode == RGUI_T(KC_H);
//     }
//
// Left GUI mod-taps morph into left Ctrl, right into right Ctrl.

#pragma once

#include <stdbool.h>
#include <stdint.h>

// Implement in the keymap; the default morphs nothing.
bool is_cmd_ctrl_morph_key(uint16_t keycode);

// Called when a morphed Ctrl is released. Returning true means someone else now owns the
// modifier (luz/mod_latch implements this), so the morph must not unregister it.
bool mod_latch_take(uint8_t mods);
