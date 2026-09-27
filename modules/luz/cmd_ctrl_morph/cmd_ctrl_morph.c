// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "cmd_ctrl_morph.h"
#include "host_os.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

__attribute__((weak)) bool is_cmd_ctrl_morph_key(uint16_t keycode) {
    return false;
}

// Without luz/mod_latch nothing takes the modifier over.
__attribute__((weak)) bool mod_latch_take(uint8_t mods) {
    return false;
}

bool process_record_cmd_ctrl_morph(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_cmd_ctrl_morph_kb(keycode, record)) {
        return false;
    }
    // Only the hold of a chosen mod-tap, and only off macOS (where GUI is already right).
    if (host_os_get() == HOST_OS_MACOS || record->tap.count || !IS_QK_MOD_TAP(keycode) ||
        !is_cmd_ctrl_morph_key(keycode)) {
        return true;
    }
    // The 5-bit mod field's 0x10 bit marks a right-hand modifier.
    const uint8_t ctrl = (QK_MOD_TAP_GET_MODS(keycode) & 0x10) ? KC_RCTL : KC_LCTL;
    if (record->event.pressed) {
        register_code(ctrl);
    } else if (!mod_latch_take(MOD_BIT(ctrl))) {
        unregister_code(ctrl);
    }
    return false;
}
