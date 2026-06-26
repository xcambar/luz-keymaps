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
// plain KC_COMM/KC_DOT), so their `?`/`!` shift can't ride a key override — a
// mod-tap never taps a custom keycode. SYM_MODTAP_SHIFT (below) supplies it from
// process_record_user. SY_COMM/SY_DOT still exist for where those symbols appear
// directly (e.g. on SYMBOLS).

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

// Companion to SYM_OVERRIDE for a symbol that lives on a mod-tap (its tap is a
// plain basic keycode, so a key override can't give it a custom shifted glyph).
// Expands to a process_record_user switch CASE: when the mod-tap is tapped while
// Shift is held, emit `shifted` instead. Drop it straight into the switch — needs
// `record` in scope, and a `default`/other cases around it as usual.
// Usage (inside process_record_user's switch):
//   SYM_MODTAP_SHIFT(RGUI_T(KC_COMM), KC_QUES)   // , → ? on the pos-32 mod-tap
#define SYM_MODTAP_SHIFT(modtap_kc, shifted) \
    case modtap_kc: \
        if (record->tap.count && record->event.pressed) { \
            uint8_t mods = get_mods() | get_oneshot_mods(); \
            if (mods & MOD_MASK_SHIFT) { \
                unregister_mods(MOD_MASK_SHIFT); \
                tap_code16(shifted); \
                register_mods(mods & MOD_MASK_SHIFT); \
                return false; \
            } \
        } \
        break;
