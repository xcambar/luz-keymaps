#include "oneshot.h"

void update_oneshot(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record,
    uint16_t *timer
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            if (*state == os_up_unqueued) {
                register_code(mod);
            }
            *state = os_down_unused;
        } else {
            switch (*state) {
            case os_down_unused:
                *state = os_up_queued;
#if ONESHOT_MOD_TIMEOUT > 0
                *timer = timer_read();
#endif
                break;
            case os_down_used:
                *state = os_up_unqueued;
                unregister_code(mod);
                break;
            default:
                break;
            }
        }
    } else {
        if (record->event.pressed) {
            if (is_oneshot_cancel_key(keycode) && *state != os_up_unqueued) {
                *state = os_up_unqueued;
                unregister_code(mod);
            }
        } else {
            if (!is_oneshot_ignored_key(keycode)) {
                switch (*state) {
                case os_down_unused:
                    *state = os_down_used;
                    break;
                case os_up_queued:
                    *state = os_up_unqueued;
                    unregister_code(mod);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

#if ONESHOT_MOD_TIMEOUT > 0
void check_oneshot_timeout(oneshot_state *state, uint16_t mod, uint16_t *timer) {
    if (*state == os_up_queued && timer_elapsed(*timer) >= ONESHOT_MOD_TIMEOUT) {
        *state = os_up_unqueued;
        unregister_code(mod);
    }
}
#endif
