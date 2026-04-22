#pragma once

#include QMK_KEYBOARD_H
#include "oneshot.h"

// Update the morphing oneshot modifier (called from process_record_user)
void update_mod_morph_oneshot(uint16_t trigger, uint16_t keycode, keyrecord_t *record);

#if ONESHOT_MOD_TIMEOUT > 0
// Check timeout for the morphing oneshot modifier (called from matrix_scan_user)
void check_mod_morph_timeout(void);
#endif
