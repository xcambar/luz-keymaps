#include "mod_morph.h"
#include "os_control.h"

// Oneshot state for the morphing modifier
oneshot_state mm_state = os_up_unqueued;
static uint16_t mm_timer = 0;

static const uint16_t mod_keys[] = {
    KC_LGUI,
    KC_LCTL,
};

void update_mod_morph_oneshot(uint16_t trigger, uint16_t keycode, keyrecord_t *record) {
    // Get the current modifier based on morph state
    uint16_t current_mod = mod_keys[get_os_platform()];

    // Use the oneshot update logic
    update_oneshot(&mm_state, current_mod, trigger, keycode, record, &mm_timer);
}

#if ONESHOT_MOD_TIMEOUT > 0
void check_mod_morph_timeout(void) {
    uint16_t current_mod = mod_keys[get_os_platform()];
    check_oneshot_timeout(&mm_state, current_mod, &mm_timer);
}
#endif
