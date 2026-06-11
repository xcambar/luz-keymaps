// Alternative SYMBOLS layer feature
// Custom keycodes with non-standard shift behavior for SYMBOLS layer
//
// Design goals:
// - Clean API: macros hide complexity
// - Compile-time initialization: key overrides generated statically

#pragma once

#include <stdint.h>
#include "quantum.h"

// Alternative symbols layer keycodes using CUSTOM_KEYCODES macro
#define ALT_SYMBOLS_LAYER_LIST CUSTOM_KEYCODES(ALT_SYMBOLS_LAYER, \
        SL_AT, \
        SL_GRV, \
        SL_EQL, \
        SL_DLR, \
        SL_AMPR, \
        SL_BSLS, \
        SL_LPRN, \
        SL_RPRN, \
        SL_LBRC, \
        SL_RBRC, \
        SL_SCLN \
    )

// Macro to generate a pair of key overrides for a symbols layer key
// Applied only to SYMBOLS layer (1 << SYMBOLS)
// Usage: SL_OVERRIDE(SL_1, KC_1, KC_AT)
#define SL_OVERRIDE(trigger, unshifted, shifted) \
    &ko_make_with_layers_and_negmods( \
        0, trigger, unshifted, 1 << SYMBOLS, MOD_MASK_SHIFT \
    ), \
    &ko_make_with_layers( \
        MOD_MASK_SHIFT, trigger, shifted, 1 << SYMBOLS \
    )

