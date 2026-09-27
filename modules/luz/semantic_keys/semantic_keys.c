// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "semantic_keys.h"
#include "host_os.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 0, 0);

#define SK_FIRST COMMUNITY_MODULE_SEMANTIC_UNDO
#define SK_LAST COMMUNITY_MODULE_SEMANTIC_HISTORY_NEXT
#define IS_SEMANTIC_KEY(kc) ((kc) >= SK_FIRST && (kc) <= SK_LAST)
#define SK(name) [COMMUNITY_MODULE_SEMANTIC_##name - SK_FIRST]

// Up to 3 chords per OS, terminated by KC_NO: {macOS}, {Linux}.
#define SEQ_LEN 3
static const uint16_t semkey_table[SK_LAST - SK_FIRST + 1][2][SEQ_LEN] = {
    SK(UNDO)            = {{G(KC_Z)},          {C(KC_Z)}},
    SK(CUT)             = {{G(KC_X)},          {C(KC_X)}},
    SK(COPY)            = {{G(KC_C)},          {C(KC_C)}},
    SK(PASTE)           = {{G(KC_V)},          {C(KC_V)}},

    // Characters (Linux: AltGr compose sequences, as on EurKEY)
    SK(EURO)            = {{LALT(S(KC_2))},    {RALT(KC_EQL), KC_E}},      // €
    SK(CEDILLA)         = {{LALT(KC_C)},       {RALT(KC_COMMA), KC_C}},    // ç

    // Motion (after HandsDown: github.com/moutis/HandsDown, moutis_semantickeys.c)
    SK(WORD_PREV)       = {{LALT(KC_LEFT)},    {C(KC_LEFT)}},
    SK(WORD_NEXT)       = {{LALT(KC_RIGHT)},   {C(KC_RIGHT)}},
    SK(LINE_BEGIN)      = {{G(KC_LEFT)},       {KC_HOME}},
    SK(LINE_END)        = {{G(KC_RIGHT)},      {KC_END}},

    // Deletion. Line deletions are GUI-tuned; shells have their own (Ctrl-U / Ctrl-K).
    SK(DEL_WORD_PREV)   = {{LALT(KC_BSPC)},    {C(KC_BSPC)}},
    SK(DEL_WORD_NEXT)   = {{LALT(KC_DEL)},     {C(KC_DEL)}},
    SK(DEL_LINE_BEGIN)  = {{G(KC_BSPC)},       {S(KC_HOME), KC_BSPC}},
    SK(DEL_LINE_END)    = {{C(KC_K)},          {S(KC_END), KC_BSPC}},

    // Browser tabs (macOS switching is Chrome/Safari's positional chord) and history
    SK(TAB_NEW)         = {{G(KC_T)},          {C(KC_T)}},
    SK(TAB_CLOSE)       = {{G(KC_W)},          {C(KC_W)}},
    SK(TAB_REOPEN)      = {{G(S(KC_T))},       {C(S(KC_T))}},
    SK(TAB_LEFT)        = {{G(A(KC_LEFT))},    {C(KC_PGUP)}},
    SK(TAB_RIGHT)       = {{G(A(KC_RGHT))},    {C(KC_PGDN)}},
    SK(HISTORY_PREV)    = {{G(KC_LBRC)},       {A(KC_LEFT)}},
    SK(HISTORY_NEXT)    = {{G(KC_RBRC)},       {A(KC_RGHT)}},
};

static const uint16_t *sequence(uint16_t sk) {
    return semkey_table[sk - SK_FIRST][host_os_get() == HOST_OS_MACOS ? 0 : 1];
}

void tap_semkey_code(uint16_t sk) {
    if (!IS_SEMANTIC_KEY(sk)) return;
    const uint16_t *seq = sequence(sk);
    for (int i = 0; i < SEQ_LEN && seq[i] != KC_NO; i++) {
        tap_code16(seq[i]);
    }
}

// The chord registered on press, released on release (single-chord keys only).
static uint16_t registered = KC_NO;

bool process_record_semantic_keys(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_semantic_keys_kb(keycode, record)) {
        return false;
    }
    if (!IS_SEMANTIC_KEY(keycode)) {
        return true;
    }
    if (record->event.pressed) {
        uint16_t sk = keycode;
        if (sk == COMMUNITY_MODULE_SEMANTIC_COPY && (get_mods() & MOD_MASK_SHIFT)) {
            sk = COMMUNITY_MODULE_SEMANTIC_CUT;  // Shift + Copy = Cut; the Shift is consumed
            del_mods(MOD_MASK_SHIFT);
        }
        const uint16_t *seq = sequence(sk);
        if (seq[1] == KC_NO) {
            register_code16(seq[0]);  // held while the key is held
            registered = seq[0];
        } else {
            tap_semkey_code(sk);
        }
    } else if (registered != KC_NO) {
        unregister_code16(registered);
        registered = KC_NO;
    }
    return false;
}
