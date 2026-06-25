// Symbol keycodes — one unified set
//
// Every symbol is a custom keycode whose output comes entirely from a pair of
// key overrides: the unshifted glyph when no shift is held, a related shifted
// glyph when shift is held. The overrides are live on ALL layers (~0), so a
// symbol works wherever it is physically placed — nothing else gates it.
//
// "Privileged" symbols are simply the ones assigned to a base-layer position;
// the rest are reached on the SYMBOLS layer. Privilege is a placement choice,
// not a property of the keycode.
//
// Exception: base positions 32/33 (`,` `.`) are mod-tap homes (RGUI_T/RALT_T on
// plain KC_COMM/KC_DOT), so their `?`/`!` shift is handled in process_record_user
// — key overrides don't fire on a mod-tap's tap keycode. SY_COMM/SY_DOT still
// exist for those symbols where they appear directly (e.g. on SYMBOLS).

#pragma once

#include <stdint.h>
#include "quantum.h"

// Symbol keycodes using the CUSTOM_KEYCODES macro from custom_keycodes.h
// Expands to: _SYM_START, SY_QUOT, ..., _SYM_COUNT
#define SYMBOLS_LIST CUSTOM_KEYCODES(SYM, \
        SY_QUOT, \
        SY_COMM, \
        SY_DOT,  \
        SY_MINS, \
        SY_UNDS, \
        SY_AT,   \
        SY_GRV,  \
        SY_EQL,  \
        SY_DLR,  \
        SY_AMPR, \
        SY_BSLS, \
        SY_LPRN, \
        SY_RPRN, \
        SY_LBRC, \
        SY_RBRC, \
        SY_SCLN  \
    )

// Generate a pair of key overrides for one symbol:
//   - unshifted output when shift is NOT held (negmods = MOD_MASK_SHIFT)
//   - shifted output when shift IS held
// Applied to all layers (~0); placement decides where the symbol is reachable.
// Usage: SYM_OVERRIDE(SY_QUOT, KC_QUOT, KC_DQUO)
#define SYM_OVERRIDE(trigger, unshifted, shifted) \
    &ko_make_with_layers_and_negmods( \
        0, trigger, unshifted, ~0, MOD_MASK_SHIFT \
    ), \
    &ko_make_with_layers( \
        MOD_MASK_SHIFT, trigger, shifted, ~0 \
    )
