#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"

/*
 * Semantic Keys - Platform Independence Layer
 *
 * This implementation provides platform-independent editing commands
 * that automatically send the correct keystroke for Mac or Linux.
 *
 * Design goals:
 * - Data-driven: keycodes stored in lookup table by platform
 * - Proper C file structure (no static variables in headers)
 *
 * Usage in keymap.c:
 * 1. Include this header in keymap.c (defines SK_* keycodes)
 * 2. Add semantic_keys.c to SRC in rules.mk
 * 3. Call process_semkey() in process_record_user()
 */

// Semantic keys list using CUSTOM_KEYCODES macro from custom_keycodes.h
// Expands to: SK_UNDO = _SEMANTIC_KEYS_START, SK_CUT, ..., _SEMANTIC_KEYS_COUNT
#define SEMANTIC_KEYS_LIST   CUSTOM_KEYCODES(SEMANTIC_KEYS, \
        SK_UNDO, \
        SK_CUT, \
        SK_COPY, \
        SK_PSTE, \
        SK_SALL, \
        SK_EURO, \
        SK_CEDIL, \
        SK_NTILDE, \
        SK_WORDPRV, \
        SK_WORDNXT, \
        SK_DOCBEG, \
        SK_DOCEND, \
        SK_LINEBEG, \
        SK_LINEEND, \
        SK_DELWORDPRV, \
        SK_DELWORDNXT, \
        SK_DELLINEBEG, \
        SK_DELLINEEND, \
        SK_TABNEW, \
        SK_TABCLOSE, \
        SK_TABREOPEN, \
        SK_TABLEFT, \
        SK_TABRIGHT, \
        SK_HISTPRV, \
        SK_HISTNXT \
    )

// Process semantic keys - call from process_record_user()
// Returns true to continue processing, false if handled
bool process_semkey(uint16_t keycode, keyrecord_t *record);

// Tap the platform-specific keycode sequence for a semantic key (for use in Compose, etc.)
void tap_semkey_code(uint16_t sk);
