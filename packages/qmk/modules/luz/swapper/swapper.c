// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Swapper: Cmd-Tab on one key, after Callum Oakley's swapper. The first tap of SW_WIN holds
// SWAPPER_MOD and taps SWAPPER_KEY; further taps only tap SWAPPER_KEY. The mod stays held
// until any other key is pressed or released, so the window list stays open while you
// pick. Shift keys don't end it (hold Shift to go backwards); override
// swapper_is_ignored_key() to change that.
//
//     SW_WIN, SW_WIN, a      -> Cmd down, Tab, Tab, Cmd up, a

#include QMK_KEYBOARD_H

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

#ifndef SWAPPER_MOD
#    define SWAPPER_MOD KC_LGUI
#endif
#ifndef SWAPPER_KEY
#    define SWAPPER_KEY KC_TAB
#endif

__attribute__((weak)) bool swapper_is_ignored_key(uint16_t keycode) {
    return keycode == KC_LSFT || keycode == KC_RSFT;
}

static bool active = false;

bool process_record_swapper(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_swapper_kb(keycode, record)) {
        return false;
    }
    if (keycode == COMMUNITY_MODULE_SWAPPER) {
        if (record->event.pressed) {
            if (!active) {
                active = true;
                register_code(SWAPPER_MOD);
            }
            register_code(SWAPPER_KEY);
        } else {
            unregister_code(SWAPPER_KEY);
        }
        return false;
    }
    if (active && !swapper_is_ignored_key(keycode)) {
        unregister_code(SWAPPER_MOD);
        active = false;
    }
    return true;
}
