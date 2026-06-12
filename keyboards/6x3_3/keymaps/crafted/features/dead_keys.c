#include QMK_KEYBOARD_H
#include "../custom_keycodes.h"
#include "dead_keys.h"
#include "os_control.h"

/*
 * Dead Keys - OS Dead Key Triggers
 *
 * This implementation sends the OS-specific dead key sequences.
 * On Linux: Uses AltGr (Right Alt) + key to trigger OS dead keys
 * On macOS: Uses Option (Left Alt) + key to trigger OS dead keys
 *
 * This is simpler and more reliable than Unicode input.
 */

// Dead key data structure: holds platform-specific keycodes
typedef struct {
    uint16_t mac_keycode;
    uint16_t linux_keycode;
} deadkey_map_t;

// Helper macros for dead key management
#define DK_beg (LIST_START_MARKER(DEAD_KEYS))
#define DK_end (LIST_LENGTH(DEAD_KEYS))
#define DK_count (DK_end - DK_beg)
#define DK_ndx(dk) ((dk) - DK_beg)
#define is_DeadKey(dk) (((dk) >= (uint16_t)(DK_beg)) && ((dk) < (uint16_t)(DK_end)))

// Lookup table for dead keys by platform
// macOS: Uses Option (Left Alt) key combinations
// Linux: Uses AltGr (Right Alt) combinations (EurKEY layout)
// These trigger the OS's compose/dead key system
// Format: [DK_ndx(keycode)] = {mac_key, linux_key}
static const deadkey_map_t deadkey_table[DK_count] = {
    [DK_ndx(DK_ACUTE)] = {LALT(KC_E),          RALT(KC_QUOT)},        // Mac: Opt+E  | Linux: AltGr+'
    [DK_ndx(DK_GRAVE)] = {LALT(KC_GRV),        RALT(KC_GRV)},         // Mac: Opt+`  | Linux: AltGr+`
    [DK_ndx(DK_CIRC)]  = {LALT(KC_I),          RALT(S(KC_6))},        // Mac: Opt+I  | Linux: AltGr+^
    [DK_ndx(DK_DIAE)]  = {LALT(KC_U),          RALT(S(KC_QUOT))},     // Mac: Opt+U  | Linux: AltGr+"
    [DK_ndx(DK_TILDE)] = {LALT(KC_N),          RALT(KC_N)},           // Mac: Opt+N  | Linux: AltGr+N
};

// Get the platform-specific keycode for a dead key (internal helper)
static uint16_t get_dead_key_code(uint16_t dk) {
    if (!is_DeadKey(dk)) {
        return KC_NO;
    }

    uint16_t idx = DK_ndx(dk);
    if (idx >= DK_count) {
        return KC_NO;
    }

    const deadkey_map_t *entry = &deadkey_table[idx];

    if (get_os_platform() == OS_MacOS) {
        return entry->mac_keycode;
    } else {
        return entry->linux_keycode;
    }
}

// Tap the platform-specific dead key for a dead key (public function)
void tap_deadkey_code(uint16_t dk) {
    if (!is_DeadKey(dk)) {
        return;
    }

    uint16_t platform_keycode = get_dead_key_code(dk);
    if (platform_keycode != KC_NO) {
        tap_code16(platform_keycode);
    }
}

// Process dead keys - call from process_record_user() BEFORE process_semkey()
bool process_dead_key(uint16_t keycode, keyrecord_t *record) {
    // Not a dead key? Continue normal processing
    if (!is_DeadKey(keycode)) {
        return true;
    }

    if (record->event.pressed) {
        tap_deadkey_code(keycode);
    }

    // Don't process this key further
    return false;
}
