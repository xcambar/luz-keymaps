// Copyright 2026 Xavier Cambar (@xcambar)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "mod_latch.h"

ASSERT_COMMUNITY_MODULES_MIN_API_VERSION(1, 1, 0);  // layer_state_set_<module>

#ifndef MOD_LATCH_LAYER
#    error "luz/mod_latch: #define MOD_LATCH_LAYER (the layer that latches) in config.h"
#endif

static uint8_t latched = 0;  // MOD_BIT mask

void mod_latch_drop(void) {
    if (latched) {
        unregister_mods(latched);
        latched = 0;
    }
}

bool mod_latch_take(uint8_t mods) {
    mods &= ~MOD_MASK_SHIFT;
    if (!mods || !layer_state_cmp(layer_state, MOD_LATCH_LAYER)) return false;
#ifdef LAYER_LOCK_ENABLE
    if (is_layer_locked(MOD_LATCH_LAYER)) return false;
#endif
    latched |= mods;
    return true;
}

bool process_record_mod_latch(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_mod_latch_kb(keycode, record)) {
        return false;
    }
#ifdef LAYER_LOCK_ENABLE
    // QK_LLCK is about to lock the latch layer (QMK locks the highest active layer, after
    // the modules run): a lock turns the chord into a mode, so let the modifiers go.
    if (keycode == QK_LAYER_LOCK && record->event.pressed &&
        get_highest_layer(layer_state) == MOD_LATCH_LAYER && !is_layer_locked(MOD_LATCH_LAYER)) {
        mod_latch_drop();
        return true;
    }
#endif
    // A mod-tap's hold being released: offer its modifiers instead of releasing them.
    if (record->event.pressed || record->tap.count != 0 || !IS_QK_MOD_TAP(keycode)) {
        return true;
    }
    // Same derivation QMK uses to register them; mod_config() honours bootmagic swaps.
    const uint8_t mod  = mod_config(QK_MOD_TAP_GET_MODS(keycode));
    const uint8_t mods = (mod & 0x10) ? ((mod & 0x0F) << 4) : (mod & 0x0F);
    return !mod_latch_take(mods);
}

layer_state_t layer_state_set_mod_latch(layer_state_t state) {
    state = layer_state_set_mod_latch_kb(state);
    if (!layer_state_cmp(state, MOD_LATCH_LAYER)) {
        mod_latch_drop();
    }
    return state;
}
