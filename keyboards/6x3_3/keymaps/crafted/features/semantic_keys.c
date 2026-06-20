#include QMK_KEYBOARD_H
#include "../custom_keycodes.h"  // Must be included before semantic_keys.h
#include "semantic_keys.h"
#include "os_control.h"

/*
 * Semantic Keys - Platform Independence Layer
 *
 * This implementation provides platform-independent editing commands
 * that automatically send the correct keystroke for Mac or Linux.
 *
 * Supports sequences of keycodes for complex inputs (e.g., dead keys + letter)
 */

// Maximum number of keycodes in a sequence
#define MAX_SEMKEY_SEQUENCE 3

// Semantic key data structure: holds platform-specific keycode sequences
// Sequences are terminated by KC_NO
typedef struct {
    uint16_t mac_sequence[MAX_SEMKEY_SEQUENCE];
    uint16_t linux_sequence[MAX_SEMKEY_SEQUENCE];
} semkey_map_t;

// Helper macros for semantic key management
#define SK_beg (LIST_START_MARKER(SEMANTIC_KEYS))
#define SK_end (LIST_LENGTH(SEMANTIC_KEYS))
#define SK_count (SK_end - SK_beg)
#define SK_ndx(sk) ((sk) - SK_beg)
#define is_SemKey(sk) (((sk) >= (uint16_t)(SK_beg)) && ((sk) < (uint16_t)(SK_end)))

// Lookup table for semantic keys by platform
// Using designated initializers (C99) for clarity - order doesn't matter!
// Format: [SK_ndx(keycode)] = {{mac_seq}, {linux_seq}}
// Sequences terminated by KC_NO
static const semkey_map_t semkey_table[SK_count] = {
    [SK_ndx(SK_UNDO)]   = {{G(KC_Z), KC_NO},              {C(KC_Z), KC_NO}},                    // Undo

    [SK_ndx(SK_CUT)]    = {{G(KC_X), KC_NO},              {C(KC_X), KC_NO}},                    // Cut
    [SK_ndx(SK_COPY)]   = {{G(KC_C), KC_NO},              {C(KC_C), KC_NO}},                    // Copy
    [SK_ndx(SK_PSTE)]   = {{G(KC_V), KC_NO},              {C(KC_V), KC_NO}},                    // Paste

    [SK_ndx(SK_SALL)]   = {{G(KC_A), KC_NO},              {C(KC_A), KC_NO}},                    // Select All

    [SK_ndx(SK_EURO)]   = {{LALT(S(KC_2)), KC_NO},        {RALT(KC_EQL), KC_E, KC_NO}},         // Euro (€): Mac: Opt+Shift+2 | Linux: AltGr+=, E
    [SK_ndx(SK_CEDIL)]  = {{LALT(KC_C), KC_NO},           {RALT(KC_COMMA), KC_C, KC_NO}},       // Cedilla (ç): Mac: Opt+C | Linux: AltGr+,, C
    [SK_ndx(SK_NTILDE)] = {{LALT(KC_N), KC_N, KC_NO},     {RALT(LSFT(KC_GRV)), KC_N, KC_NO}},           // Ñ: Mac: Opt+N, N | Linux: AltGr+N, N

    // Navigation keys - taken from HandsDown
    // https://github.com/moutis/HandsDown/blob/ac766b44b1454d72b395ee460bcae9f6fe89a700/moutis_semantickeys.c
    [SK_ndx(SK_WORDPRV)] = {{LALT(KC_LEFT), KC_NO},       {C(KC_LEFT), KC_NO}},      // WORD LEFT
    [SK_ndx(SK_WORDNXT)] = {{LALT(KC_RIGHT), KC_NO},      {C(KC_RIGHT), KC_NO}},     // WORD RIGHT
    [SK_ndx(SK_DOCBEG)]  = {{G(KC_UP), KC_NO},            {C(KC_HOME), KC_NO}},      // Go to start of document
    [SK_ndx(SK_DOCEND)]  = {{G(KC_DOWN), KC_NO},          {C(KC_END), KC_NO}},       // Go to end of document
    [SK_ndx(SK_LINEBEG)] = {{G(KC_LEFT), KC_NO},          {KC_HOME, KC_NO}},         // Go to beginning of line
    [SK_ndx(SK_LINEEND)] = {{G(KC_RIGHT), KC_NO},         {KC_END, KC_NO}},          // Go to end of line

    // Deletion keys (NAV_DEL sub-layer)
    // Line deletions are GUI-tuned; shells use their own bindings (Ctrl-U / Ctrl-K)
    [SK_ndx(SK_DELWORDPRV)] = {{LALT(KC_BSPC), KC_NO},    {C(KC_BSPC), KC_NO}},          // Delete word backward
    [SK_ndx(SK_DELWORDNXT)] = {{LALT(KC_DEL), KC_NO},     {C(KC_DEL), KC_NO}},           // Delete word forward
    [SK_ndx(SK_DELLINEBEG)] = {{G(KC_BSPC), KC_NO},       {S(KC_HOME), KC_BSPC, KC_NO}}, // Delete to line beginning
    [SK_ndx(SK_DELLINEEND)] = {{C(KC_K), KC_NO},          {S(KC_END), KC_BSPC, KC_NO}},  // Delete to line end

    // Browser tab management (TABS sub-layer). macOS switch chords are Chrome/Safari positional.
    [SK_ndx(SK_TABNEW)]    = {{G(KC_T), KC_NO},           {C(KC_T), KC_NO}},                  // New tab:    Cmd/Ctrl+T
    [SK_ndx(SK_TABCLOSE)]  = {{G(KC_W), KC_NO},           {C(KC_W), KC_NO}},                  // Close tab:  Cmd/Ctrl+W
    [SK_ndx(SK_TABREOPEN)] = {{G(S(KC_T)), KC_NO},        {C(S(KC_T)), KC_NO}},               // Reopen tab: Cmd/Ctrl+Shift+T
    [SK_ndx(SK_TABLEFT)]   = {{G(A(KC_LEFT)), KC_NO},     {C(KC_PGUP), KC_NO}},               // Tab left:   Cmd+Opt+Left / Ctrl+PgUp
    [SK_ndx(SK_TABRIGHT)]  = {{G(A(KC_RGHT)), KC_NO},     {C(KC_PGDN), KC_NO}},               // Tab right:  Cmd+Opt+Right / Ctrl+PgDn

    // Browser history navigation (prev/next page). Firefox & Chrome on both platforms.
    [SK_ndx(SK_HISTPRV)]   = {{G(KC_LBRC), KC_NO},        {A(KC_LEFT), KC_NO}},               // Page back:    Cmd+[ / Alt+Left
    [SK_ndx(SK_HISTNXT)]   = {{G(KC_RBRC), KC_NO},        {A(KC_RGHT), KC_NO}},               // Page forward: Cmd+] / Alt+Right

    // Add Warpd later...
};

// Tap a sequence of keycodes for a semantic key (public function)
void tap_semkey_code(uint16_t sk) {
    if (!is_SemKey(sk)) {
        return;
    }

    uint16_t idx = SK_ndx(sk);
    if (idx >= SK_count) {
        return;
    }

    const semkey_map_t *entry = &semkey_table[idx];
    const uint16_t *sequence = (get_os_platform() == OS_MacOS) ? entry->mac_sequence : entry->linux_sequence;

    // Tap each keycode in the sequence until we hit KC_NO
    for (int i = 0; i < MAX_SEMKEY_SEQUENCE && sequence[i] != KC_NO; i++) {
        tap_code16(sequence[i]);
    }
}

// Get the first platform-specific keycode for a semantic key (for backwards compatibility)
// Returns KC_NO for multi-keycode sequences
uint16_t get_semkey_code(uint16_t sk) {
    if (!is_SemKey(sk)) {
        return KC_NO;
    }

    uint16_t idx = SK_ndx(sk);
    if (idx >= SK_count) {
        return KC_NO;
    }

    const semkey_map_t *entry = &semkey_table[idx];
    const uint16_t *sequence = (get_os_platform() == OS_MacOS) ? entry->mac_sequence : entry->linux_sequence;

    // Return first keycode if it's the only one, otherwise KC_NO (use tap_semkey_code instead)
    if (sequence[0] != KC_NO && sequence[1] == KC_NO) {
        return sequence[0];
    }

    return KC_NO;  // Multi-keycode sequence, can't return a single code
}

// Track the registered semantic key for proper release
static uint16_t registered_semkey_code = KC_NO;

// Process semantic keys - call from process_record_user()
// Returns true to continue processing, false if handled
//
// Shift morphing: reads get_mods() to detect shift, clears it when consumed
bool process_semkey(uint16_t keycode, keyrecord_t *record) {
    // Not a semantic key? Continue normal processing
    if (!is_SemKey(keycode)) {
        return true;
    }

    if (record->event.pressed) {
        // Check current modifier state for shift morphing
        uint8_t mods = get_mods();

        // Check for shift morphing: SK_COPY + Shift → SK_CUT
        uint16_t actual_keycode = keycode;
        if (keycode == SK_COPY && (mods & MOD_MASK_SHIFT)) {
            actual_keycode = SK_CUT;
            // Clear shift - we've consumed it for the morph
            del_mods(MOD_MASK_SHIFT);
        }

        // Get the platform-specific keycode
        uint16_t platform_keycode = get_semkey_code(actual_keycode);

        // If it's a single keycode, use register/unregister for proper hold behavior
        if (platform_keycode != KC_NO) {
            register_code16(platform_keycode);
            registered_semkey_code = platform_keycode;
        } else {
            // Multi-keycode sequence - just tap it
            tap_semkey_code(actual_keycode);
        }
    } else {
        // Key release: unregister whatever we sent on keydown
        if (registered_semkey_code != KC_NO) {
            unregister_code16(registered_semkey_code);
            registered_semkey_code = KC_NO;
        }
    }

    // Don't process this key further
    return false;
}
