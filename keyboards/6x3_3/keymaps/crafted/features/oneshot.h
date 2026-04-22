#pragma once

#include QMK_KEYBOARD_H

// Represents the four states a oneshot key can be in
typedef enum {
    os_up_unqueued,
    os_up_queued,
    os_down_unused,
    os_down_used,
} oneshot_state;

// Callum-style oneshot mod timeout (ms). Set to 0 to disable.
#ifndef ONESHOT_MOD_TIMEOUT
#define ONESHOT_MOD_TIMEOUT 0
#endif

// Custom oneshot mod implementation that doesn't rely on timers. If a mod is
// used while it is held it will be unregistered on keyup as normal, otherwise
// it will be queued and only released after the next non-mod keyup.
void update_oneshot(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record,
    uint16_t *timer
);

#if ONESHOT_MOD_TIMEOUT > 0
// Check if a queued oneshot mod has timed out and cancel it.
// Call from matrix_scan_user(). Pass the same timer variable used in update_oneshot.
void check_oneshot_timeout(oneshot_state *state, uint16_t mod, uint16_t *timer);
#endif

// To be implemented by the consumer. Defines keys to cancel oneshot mods.
bool is_oneshot_cancel_key(uint16_t keycode);

// To be implemented by the consumer. Defines keys to ignore when determining
// whether a oneshot mod has been used. Setting this to modifiers and layer
// change keys allows stacking multiple oneshot modifiers, and carrying them
// between layers.
bool is_oneshot_ignored_key(uint16_t keycode);
