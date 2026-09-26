// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "dead_keys.h"
#include "host_os.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

#define DK_FIRST COMMUNITY_MODULE_DEAD_ACUTE
#define DK_LAST COMMUNITY_MODULE_DEAD_TILDE
#define IS_DEAD_KEY(kc) ((kc) >= DK_FIRST && (kc) <= DK_LAST)

// The host's dead-key chord per accent: {macOS, Linux}.
static const uint16_t deadkey_table[DK_LAST - DK_FIRST + 1][2] = {
    [COMMUNITY_MODULE_DEAD_ACUTE - DK_FIRST] = {LALT(KC_E), RALT(KC_QUOT)},      // ´  Opt+E   | AltGr+'
    [COMMUNITY_MODULE_DEAD_GRAVE - DK_FIRST] = {LALT(KC_GRV), RALT(KC_GRV)},     // `  Opt+`   | AltGr+`
    [COMMUNITY_MODULE_DEAD_CIRC - DK_FIRST]  = {LALT(KC_I), RALT(S(KC_6))},      // ^  Opt+I   | AltGr+^
    [COMMUNITY_MODULE_DEAD_DIAE - DK_FIRST]  = {LALT(KC_U), RALT(S(KC_QUOT))},   // ¨  Opt+U   | AltGr+"
    [COMMUNITY_MODULE_DEAD_TILDE - DK_FIRST] = {LALT(KC_N), RALT(S(KC_GRV))},    // ~  Opt+N   | AltGr+~
};

void tap_deadkey_code(uint16_t dk) {
    if (!IS_DEAD_KEY(dk)) return;
    tap_code16(deadkey_table[dk - DK_FIRST][host_os_get() == HOST_OS_MACOS ? 0 : 1]);
}

bool process_record_dead_keys(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_dead_keys_kb(keycode, record)) {
        return false;
    }
    if (!IS_DEAD_KEY(keycode)) {
        return true;
    }
    if (record->event.pressed) {
        tap_deadkey_code(keycode);
    }
    return false;
}
