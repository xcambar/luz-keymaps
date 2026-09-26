// Luz — the shared keymap.c body
// ----------------------------------------------------------------------------
// Canonical file at keyboards/6x3_3/luz/luz.h, shared by every Luz variant. A variant's
// keymap.c holds only what is per-layout (keymaps[] and the position-bound keycodes below),
// then includes this file LAST, after keymaps[]:
//
//     #define LUZ_MORPH_L    LGUI_T(...)    // inner-index morph keys (luz/cmd_ctrl_morph)
//     #define LUZ_MORPH_R    RGUI_T(...)
//     #define SY_COMM_MODTAP RGUI_T(...)    // mod-taps with a custom shifted glyph
//     #define SY_DOT_MODTAP  RALT_T(...)    // (SYM_MODTAP_SHIFT, luz/symbols.h)
//     #define LUZ_VARIANT_MODTAP_SHIFTS ... // optional: extra SYM_MODTAP_SHIFT cases
//     const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { ... };
//     #include "luz/luz.h"
//
// The OS-aware features are QMK community modules (modules/luz/*), wired by each variant's
// keymap.json in this order, which is the order they see every key in:
//     compose, dead_keys, semantic_keys, swapper, cmd_ctrl_morph, mod_latch, host_os
// What stays here is Luz's own glue: the Compose combo, the symbol set's key overrides,
// the ADJUST tri-layer, the macros and Caps Word.

#pragma once

#include "luz/compose_combo.h"
#include "luz/symbols.h"
#include "luz/mods.h"
#include "compose.h"   // luz/compose module: compose_arm()

#ifndef LUZ_VARIANT_MODTAP_SHIFTS
#    define LUZ_VARIANT_MODTAP_SHIFTS
#endif

_Static_assert(MOD_LATCH_LAYER == SYMBOLS, "luz/config.h: MOD_LATCH_LAYER must be the SYMBOLS layer");

// Combo indices — must match key_combos[] order exactly
enum combo_events {
    COMBO_COMPOSE,
};

combo_t key_combos[] = {
    COMBO_ACTION(compose_combo), // COMBO_COMPOSE
};

// Key Overrides — generated from SYMBOL_TABLE (one unshifted/shifted pair per
// symbol, on all layers). The table and the SY_*_SHIFTED constants it references
// live in luz/symbols.h; add a symbol there, not here.
const key_override_t* key_overrides[] = {
    SYMBOL_TABLE(SYM_OVR)
    NULL
};

// luz/cmd_ctrl_morph: the inner-index GUI mod-taps hold Ctrl off macOS.
bool is_cmd_ctrl_morph_key(uint16_t keycode) {
    return keycode == LUZ_MORPH_L || keycode == LUZ_MORPH_R;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // ADJUST tri-layer: active while both EXTEND and SYMBOLS are held
    return update_tri_layer_state(state, EXTEND, SYMBOLS, ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MD_FENCE:
            if (record->event.pressed) {
                uint8_t saved = get_mods();
                clear_mods();
                SEND_STRING("```");
                set_mods(saved);
            }
            return false;

        case ARROW_OP:
            if (record->event.pressed) {
                uint8_t saved = get_mods();
                clear_mods();
                send_string((saved & MOD_MASK_SHIFT) ? "=>" : "->");
                set_mods(saved);
            }
            return false;

        // Custom shifted glyph for the , / . mod-taps (pos 32/33), plus any the variant
        // adds via LUZ_VARIANT_MODTAP_SHIFTS; logic in symbols.h
        SYM_MODTAP_SHIFT(COMM) // , → ?
        SYM_MODTAP_SHIFT(DOT)  // . → !
        LUZ_VARIANT_MODTAP_SHIFTS
    }
    return true;
}

// Combo event handler
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case COMBO_COMPOSE:
            if (pressed) {
                compose_arm();
            }
            break;
    }
}

// Caps Word: QMK default, plus SY_MINS so SCREAMING_SNAKE_CASE survives the underscore.
// `_` is the shifted face of the SY_MINS (`-`) key, so weak-shifting SY_MINS makes it
// emit `_` under Caps Word (the override turns the held shift into KC_UNDS).
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Continue Caps Word, applying shift (uppercases letters; turns `-` into `_`):
        case KC_A ... KC_Z:
        case KC_MINS:
        case SY_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        // Continue Caps Word without shifting:
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
            return true;
        default:
            return false;  // any other key ends Caps Word
    }
}
