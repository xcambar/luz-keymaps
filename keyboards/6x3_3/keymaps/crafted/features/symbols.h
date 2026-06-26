// Symbol keycodes — one unified set, defined by a single table
//
// Every symbol is a custom keycode (SY_*) whose output comes entirely from a pair
// of key overrides: the unshifted glyph when no shift is held, a related shifted
// glyph when shift is held. The overrides are live on ALL layers (~0), so a symbol
// works wherever it is physically placed — nothing else gates it.
//
// SYMBOL_TABLE below is the single source of truth: one row per symbol listing the
// shifted partner. The unshifted output is always KC_<NAME>, so both the keycode
// enum (custom_keycodes.h) and the key_overrides[] array (keymap.c) are generated
// from it — add a symbol by adding one row. Names therefore match their unshifted
// keycode (e.g. SY_LCBR for `{` = KC_LCBR, not SY_LBRC).
//
// "Privileged" symbols are simply the ones assigned to a base-layer position; the
// rest are reached on the SYMBOLS layer. Privilege is a placement choice.
//
// Exception: base positions 32/33 (`,` `.`) are mod-tap homes (RGUI_T/RALT_T on
// plain KC_COMM/KC_DOT), so their `?`/`!` shift can't ride a key override — a
// mod-tap never taps a custom keycode. SYM_MODTAP_SHIFT (below) supplies it from
// process_record_user, reading the same SY_COMM_SHIFTED/SY_DOT_SHIFTED the table does.

#pragma once

#include <stdint.h>
#include "quantum.h"

// Shifted partners for the , / . mod-taps — named so both the table row and the
// SYM_MODTAP_SHIFT handler in process_record_user read one definition.
#define SY_COMM_SHIFTED KC_QUES   // , → ?
#define SY_DOT_SHIFTED  KC_EXLM   // . → !

// The symbol set: X(NAME, shifted). Unshifted output is always KC_<NAME>.
#define SYMBOL_TABLE(X)                       \
    X(QUOT, KC_DQUO)          /* ' -> " */    \
    X(MINS, KC_UNDS)          /* - -> _ */    \
    X(SLSH, KC_PIPE)          /* / -> | */    \
    X(COMM, SY_COMM_SHIFTED)  /* , -> ? */    \
    X(DOT,  SY_DOT_SHIFTED)   /* . -> ! */    \
    X(AT,   KC_HASH)          /* @ -> # */    \
    X(GRV,  KC_TILD)          /* ` -> ~ */    \
    X(EQL,  KC_PLUS)          /* = -> + */    \
    X(DLR,  KC_PERC)          /* $ -> % */    \
    X(AMPR, KC_ASTR)          /* & -> * */    \
    X(BSLS, KC_CIRC)          /* \ -> ^ */    \
    X(LPRN, KC_LT)            /* ( -> < */    \
    X(RPRN, KC_GT)            /* ) -> > */    \
    X(LCBR, KC_LBRC)          /* { -> [ */    \
    X(RCBR, KC_RBRC)          /* } -> ] */    \
    X(COLN, KC_SCLN)          /* : -> ; */

// Keycode enum entries — consumed by `enum custom_keycodes` in custom_keycodes.h.
// Expands to: _SYM_START, SY_QUOT, ..., _SYM_COUNT
#define SYM_KEYCODE(name, shifted) SY_##name,
#define SYMBOLS_LIST _SYM_START, SYMBOL_TABLE(SYM_KEYCODE) _SYM_COUNT

// Generate a pair of key overrides for one symbol:
//   - unshifted output when shift is NOT held (negmods = MOD_MASK_SHIFT)
//   - shifted output when shift IS held
// Applied to all layers (~0); placement decides where the symbol is reachable.
#define SYM_OVERRIDE(trigger, unshifted, shifted) \
    &ko_make_with_layers_and_negmods( \
        0, trigger, unshifted, ~0, MOD_MASK_SHIFT \
    ), \
    &ko_make_with_layers( \
        MOD_MASK_SHIFT, trigger, shifted, ~0 \
    )

// One key_overrides[] pair per symbol — consumed by the array in keymap.c via
// SYMBOL_TABLE(SYM_OVR). Unshifted is KC_<name> (matches the keycode name).
#define SYM_OVR(name, shifted) SYM_OVERRIDE(SY_##name, KC_##name, shifted),

// Companion to SYM_OVERRIDE for a symbol that lives on a mod-tap (its tap is a
// plain basic keycode, so a key override can't give it a custom shifted glyph).
// Expands to a process_record_user switch CASE: when the mod-tap is tapped while
// Shift is held, emit the symbol's shifted glyph instead. Single arg is the symbol
// NAME; it derives both halves by paste:
//   - case keycode  = SY_<NAME>_MODTAP   (the mod-tap keycode; define in keymap.c,
//                      since which modifier/position it uses is a keymap decision)
//   - shifted glyph = SY_<NAME>_SHIFTED  (from the table above)
// Drop into the switch — needs `record` in scope.
// Usage (inside process_record_user's switch):
//   SYM_MODTAP_SHIFT(COMM)   // , → ? for SY_COMM on its base mod-tap
#define SYM_MODTAP_SHIFT(name) \
    case SY_##name##_MODTAP: \
        if (record->tap.count && record->event.pressed) { \
            uint8_t mods = get_mods() | get_oneshot_mods(); \
            if (mods & MOD_MASK_SHIFT) { \
                unregister_mods(MOD_MASK_SHIFT); \
                tap_code16(SY_##name##_SHIFTED); \
                register_mods(mods & MOD_MASK_SHIFT); \
                return false; \
            } \
        } \
        break;
