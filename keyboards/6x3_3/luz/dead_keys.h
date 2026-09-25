#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"

/*
 * Dead Keys - Accent System
 *
 * This implementation provides dead key functionality for typing accented characters.
 * Press an accent key (e.g., DK_ACUTE) followed by a letter to produce an accented character.
 *
 * Design goals:
 * - Platform-independent using Unicode input
 * - Data-driven: accent combinations stored in lookup tables
 *
 * Wired in luz/luz.h (process_dead_key() before process_semkey()); built via luz/rules.mk.
 */

// Dead keys list using CUSTOM_KEYCODES macro from custom_keycodes.h
// Expands to: _DEAD_KEYS_START, DK_ACUTE, ..., _DEAD_KEYS_COUNT
#define DEAD_KEYS_LIST CUSTOM_KEYCODES(DEAD_KEYS, \
        DK_ACUTE, \
        DK_GRAVE, \
        DK_CIRC, \
        DK_DIAE, \
        DK_TILDE \
    )

// Process dead keys - call from process_record_user() BEFORE process_semkey()
// Returns true to continue processing, false if handled
bool process_dead_key(uint16_t keycode, keyrecord_t *record);

// Tap the platform-specific dead key (for use in Compose, etc.)
void tap_deadkey_code(uint16_t dk);
