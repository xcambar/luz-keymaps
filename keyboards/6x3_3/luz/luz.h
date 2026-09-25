// Luz — the shared keymap.c body
// ----------------------------------------------------------------------------
// Canonical file at keyboards/6x3_3/luz/luz.h, shared by every Luz variant. A variant's
// keymap.c holds only what is per-layout (keymaps[] and the position-bound keycodes below),
// then includes this file LAST, after keymaps[]:
//
//     #define LUZ_MORPH_L    LGUI_T(...)    // inner-index morph keys (see luz/mods.h)
//     #define LUZ_MORPH_R    RGUI_T(...)
//     #define SY_COMM_MODTAP RGUI_T(...)    // mod-taps with a custom shifted glyph
//     #define SY_DOT_MODTAP  RALT_T(...)    // (SYM_MODTAP_SHIFT, luz/symbols.h)
//     #define LUZ_VARIANT_MODTAP_SHIFTS ... // optional: extra SYM_MODTAP_SHIFT cases
//     const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = { ... };
//     #include "luz/luz.h"
//
// Everything else (Compose, the combo and key-override tables, dead and semantic keys, the
// swapper, the Cmd/Ctrl morph, the mod latch, Caps Word) is identical in every variant.

#pragma once

#include "luz/swapper.h"
#include "luz/os_control.h"
#include "luz/compose.h"
#include "luz/semantic_keys.h"
#include "luz/dead_keys.h"
#include "luz/symbols.h"

#ifndef LUZ_VARIANT_MODTAP_SHIFTS
#    define LUZ_VARIANT_MODTAP_SHIFTS
#endif

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

// Luz shared mod system: chordal_hold_layout (positional) + the Cmd/Ctrl morph
#include "luz/mods.h"

// Snapshot the morph keycodes for LUZ_MORPH_KEY (the preprocessor expands them here).
static const uint16_t morph_l = LUZ_MORPH_L;
static const uint16_t morph_r = LUZ_MORPH_R;

// Compose state: armed by the Shift+Space thumb combo, consumed by the next keypress
static bool compose_pending = false;

// Swapper state
static bool sw_win_active = false;

layer_state_t layer_state_set_user(layer_state_t state) {
    // ADJUST tri-layer: active while both EXTEND and SYMBOLS are held
    state = update_tri_layer_state(state, EXTEND, SYMBOLS, ADJUST);
    // Layer-scoped mod latch: leaving SYMBOLS releases whatever it is holding (mod_latch.h)
    luz_mod_latch_layer_state(state);
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Compose (accents): the next key picks a dead key or special character.
    // Unlike the old Leader, unmatched keys pass through unchanged.
    if (compose_pending && record->event.pressed) {
        uint16_t kc = keycode;
        if (IS_QK_MOD_TAP(keycode)) {
            if (record->tap.count == 0) {
                return true;  // mod-tap held as modifier: keep compose pending
            }
            kc = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        }
        switch (kc) {
            case KC_LCTL ... KC_RGUI:
                return true;  // plain modifiers don't consume compose (allows shifted accents)
            case KC_ESC:
                compose_pending = false;
                return false;  // cancel
            case KC_E: compose_pending = false; tap_deadkey_code(DK_ACUTE); return false;
            case KC_A: compose_pending = false; tap_deadkey_code(DK_GRAVE); return false;
            case KC_U: compose_pending = false; tap_deadkey_code(DK_DIAE);  return false;
            case KC_O: compose_pending = false; tap_deadkey_code(DK_CIRC);  return false;
            case KC_C: compose_pending = false; tap_semkey_code(SK_CEDIL);  return false;
            case KC_N: compose_pending = false; tap_deadkey_code(DK_TILDE); return false;
            case KC_W: compose_pending = false; tap_semkey_code(SK_EURO);   return false;
            default:
                compose_pending = false;
                return true;  // pass through unchanged
        }
    }

    // Process dead keys FIRST (before semantic keys and oneshots)
    if (!process_dead_key(keycode, record)) {
        return false;  // Dead key was handled
    }

    // Process semantic keys (platform-independent editing commands)
    if (!process_semkey(keycode, record)) {
        return false;  // Semantic key was handled
    }

    // Update swapper
    update_swapper(&sw_win_active, KC_LGUI, KC_TAB, SW_WIN, keycode, record);

    // OS morph: inner-index mod-taps (LUZ_MORPH_L/R) use GUI on macOS, Ctrl on Linux
    LUZ_MORPH_KEY(keycode, record, morph_l, KC_LCTL);
    LUZ_MORPH_KEY(keycode, record, morph_r, KC_RCTL);

    // Layer-scoped mod latch: under SYMBOLS, releasing a held mod-tap latches the mod for
    // the life of the layer instead of releasing it (Shift excluded; see luz/mod_latch.h).
    if (!luz_mod_latch_process(keycode, record)) {
        return false;
    }

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

        // System actions on ADJUST (moved off the old base-layer combos)
        case SW_OS:
            if (record->event.pressed) {
                toggle_os_platform();
            }
            return false;

        case PR_OS:
            if (record->event.pressed) {
                send_string(get_os_platform_name());
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

// Define keys that should be ignored by swapper (allows changing direction)
bool is_swapper_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case KC_LSFT:
        case KC_RSFT:
            return true;
        default:
            return false;
    }
}

// Combo event handler
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case COMBO_COMPOSE:
            if (pressed) {
                compose_pending = true;
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
