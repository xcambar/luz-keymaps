#include "swapper.h"

void update_swapper(
    bool *active,
    uint16_t cmdish,
    uint16_t tabish,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            if (!*active) {
                *active = true;
                register_code(cmdish);
            }
            // Shift is already registered by oneshot or held key, just tap tabish
            register_code(tabish);
        } else {
            unregister_code(tabish);
            // Don't unregister cmdish until some other key is hit or released.
        }
    } else if (*active) {
        // Don't deactivate if it's an ignored key (e.g., shift for direction change)
        if (is_swapper_ignored_key(keycode)) {
            return;
        }
        unregister_code(cmdish);
        *active = false;
    }
}
