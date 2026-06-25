// Alternative base layer symbols feature
// Data-driven key override generation for custom shift morphing
//
// Design goals:
// - Data-driven: symbol mappings stored in a lookup table
// - Clean API: macros hide complexity
// - Compile-time initialization: key overrides generated statically

#pragma once

#include <stdint.h>
#include "quantum.h"

// Alternative symbols list using CUSTOM_KEYCODES macro from custom_keycodes.h
// Expands to: _ALT_SYMBOLS_START, AS_QUOT, ..., _ALT_SYMBOLS_COUNT
#define ALT_SYMBOLS_LIST CUSTOM_KEYCODES(ALT_SYMBOLS, \
        AS_QUOT, \
        AS_COMM, \
        AS_DOT, \
        AS_MINS, \
        AS_UNDS \
    )

// Macro to generate a pair of key overrides for an alt symbol
// Creates both unshifted (with negmods) and shifted versions
// Applied to all layers (~0)
// NOTE: We still need explicit unshifted/shifted parameters because
// the config table isn't available at compile-time for macro expansion
// Usage: ALT_SYMBOL_OVERRIDE(AS_QUOT, KC_QUOT, KC_DQUO)
#define ALT_SYMBOL_OVERRIDE(trigger, unshifted, shifted) \
    &ko_make_with_layers_and_negmods( \
        0, trigger, unshifted, ~0, MOD_MASK_SHIFT \
    ), \
    &ko_make_with_layers( \
        MOD_MASK_SHIFT, trigger, shifted, ~0 \
    )
