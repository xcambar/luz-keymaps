// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "compose.h"
#include "dead_keys.h"
#include "semantic_keys.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

static bool armed = false;

void compose_arm(void) {
    armed = true;
}

bool compose_is_armed(void) {
    return armed;
}

bool process_record_compose(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_compose_kb(keycode, record)) {
        return false;
    }
    if (!armed || !record->event.pressed) {
        return true;
    }
    uint16_t kc = keycode;
    if (IS_QK_MOD_TAP(keycode)) {
        if (record->tap.count == 0) {
            return true;  // a mod-tap held as its modifier: stay armed
        }
        kc = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
    }
    switch (kc) {
        case KC_LCTL ... KC_RGUI:
            return true;  // plain modifiers don't consume compose (Shift+E -> É)
        case KC_ESC:
            armed = false;
            return false;
        case KC_E: armed = false; tap_deadkey_code(COMMUNITY_MODULE_DEAD_ACUTE);        return false;
        case KC_A: armed = false; tap_deadkey_code(COMMUNITY_MODULE_DEAD_GRAVE); return false;
        case KC_U: armed = false; tap_deadkey_code(COMMUNITY_MODULE_DEAD_DIAE); return false;
        case KC_O: armed = false; tap_deadkey_code(COMMUNITY_MODULE_DEAD_CIRC); return false;
        case KC_N: armed = false; tap_deadkey_code(COMMUNITY_MODULE_DEAD_TILDE); return false;
        case KC_C: armed = false; tap_semkey_code(COMMUNITY_MODULE_SEMANTIC_CEDILLA); return false;
        case KC_W: armed = false; tap_semkey_code(COMMUNITY_MODULE_SEMANTIC_EURO); return false;
        default:
            armed = false;
            return true;  // any other key passes through unchanged
    }
}
