// Luz — shared symbol set
// ----------------------------------------------------------------------------
// Canonical file at keyboards/6x3_3/luz/symbols.h, shared by every Luz variant.
// Each variant's rules.mk puts this dir's parent on the include path
// (`VPATH += $(QMK_USERSPACE)/keyboards/6x3_3`), so custom_keycodes.h / keymap.c can
// `#include "luz/symbols.h"`. See LUZ.md (repo root) for the prose spec.
//
// The symbol VOCABULARY and BEHAVIOR are a Luz contract, shared verbatim by every
// variant so muscle memory transfers. What stays per-layout is PLACEMENT — which
// symbols are privileged onto BASE positions and how the SYMBOLS field is arranged.
// Placement lives in each keymap.c, not here.
//
// Every symbol is a custom keycode (SY_*) whose output comes entirely from a pair
// of key overrides: the unshifted glyph when no shift is held, a RELATED shifted
// glyph when shift is held (e.g. ( -> <, { -> [, = -> +). The overrides are live on
// ALL layers (~0), so a symbol works wherever it is physically placed — nothing
// else gates it.
//
// SYMBOL_TABLE below is the single source of truth: one row per symbol listing the
// shifted partner. The unshifted output is always KC_<NAME>, so both the keycode
// enum (custom_keycodes.h) and the key_overrides[] array (keymap.c) are generated
// from it — add a symbol by adding one row, and it appears in every variant. Names
// therefore match their unshifted keycode (e.g. SY_LCBR for `{` = KC_LCBR, not SY_LBRC).
//
// Mod-tap exception: a variant may place a symbol on a BASE mod-tap position (e.g.
// `'`/`,`/`.` on a home-/bottom-row mod-tap). A mod-tap's tap is a plain basic
// keycode, so it can't ride a key override for its shifted partner — SYM_MODTAP_SHIFT
// (below) supplies it from process_record_user instead, reading the same
// SY_*_SHIFTED constants the table uses. WHICH symbols sit on mod-taps (and thus
// which SYM_MODTAP_SHIFT cases / SY_*_MODTAP defines exist) is a per-layout choice
// in keymap.c. The three punctuation symbols that can take that role have their
// shifted partner named below so both the table row and the handler read one define.

#pragma once

#include <stdint.h>
#include "quantum.h"

// Shifted partners for the punctuation symbols that may live on a mod-tap — named
// so both the table row and the SYM_MODTAP_SHIFT handler read one definition.
#define SY_QUOT_SHIFTED KC_DQUO   // ' → "
#define SY_COMM_SHIFTED KC_QUES   // , → ?
#define SY_DOT_SHIFTED  KC_EXLM   // . → !

// The symbol set: X(NAME, shifted). Unshifted output is always KC_<NAME>.
#define SYMBOL_TABLE(X)                       \
    X(QUOT, SY_QUOT_SHIFTED)  /* ' -> " */    \
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
