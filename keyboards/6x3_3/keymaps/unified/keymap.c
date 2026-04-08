// Copyright 2025 Xavier (@xavier)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Unified keymap: combines zen and crafted with runtime switching via Z+M combo.
// Starts in crafted mode. Press Z+M to toggle to zen mode and back.

/*
 * Keyboard Layout - Abstract Key Positions (_XX_)
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │ _00_│ _01_│ _02_│ _03_│ _04_│ _05_│       │ _06_│ _07_│ _08_│ _09_│ _10_│ _11_│
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │ _12_│ _13_│ _14_│ _15_│ _16_│ _17_│       │ _18_│ _19_│ _20_│ _21_│ _22_│ _23_│
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │ _24_│ _25_│ _26_│ _27_│ _28_│ _29_│       │ _30_│ _31_│ _32_│ _33_│ _34_│ _35_│
 * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
 *                     ┌─────┐                           ┌─────┐
 *                     │ _36_├─────┐               ┌─────┤ _41_│
 *                     └─────┤ _37_├─────┐   ┌─────┤ _40_├─────┘
 *                           └─────┤ _38_│   │ _39_├─────┘
 *                                 └─────┘   └─────┘
 */

#include "keymap_eurkey.h"
#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>

// Custom keycodes (must be included before feature_base_layer.h)
#include "custom_keycodes.h"

// Base layer switch
#include "feature_base_layer.h"

// Weak corners feature
#include "feature_weak_corners.h"

// Callum-style oneshot modifiers
#include "features/oneshot.h"

// Callum-style swapper
#include "features/swapper.h"

// Modifier morph (GUI/Ctrl toggle)
#include "features/mod_morph.h"

// OS control for platform-aware features
#include "features/os_control.h"

// ============================================================================
// Layer definitions
// ============================================================================

// Crafted layers (0-6) — C_BASE must be layer 0 for COMBO_ONLY_FROM_LAYER
enum layers {
    C_BASE = 0,
    C_BASE_ALT,
    C_FAVS,
    C_NAV,
    C_NAV_FASTER,
    C_NAV_FASTEST,
    C_SYMBOLS,
    Z_BASE,
    Z_CODE,
    Z_NAV,
};

// Layer name aliases so crafted's feature headers resolve correctly
#define BASE        C_BASE
#define BASE_ALT    C_BASE_ALT
#define FAVS        C_FAVS
#define NAV         C_NAV
#define NAV_FASTER  C_NAV_FASTER
#define NAV_FASTEST C_NAV_FASTEST
#define SYMBOLS     C_SYMBOLS

// Mode tracking
static bool zen_mode = false;

// Include semantic keys header
#include "features/semantic_keys.h"

// Include dead keys header
#include "features/dead_keys.h"

// Include alternative symbols header
#include "features/alt_symbols.h"
#include "features/alt_symbols_layer.h"

// ============================================================================
// Combos
// ============================================================================

enum combo_events {
    COMBO_TOGGLE_MODE,
    COMBO_SWITCH_OS,
    COMBO_PRINT_OS,
    COMBO_SWITCH_LAYOUT,
    COMBO_PRINT_LAYOUT,
#ifdef XC_WEAK_CORNERS
    COMBO_WC_TL,
    COMBO_WC_TR,
    COMBO_WC_BL,
    COMBO_WC_BR,
#endif
};

const uint16_t PROGMEM boot_combo[] = {KC_TAB, KC_BSPC, COMBO_END};
const uint16_t PROGMEM toggle_mode_combo[] = {KC_Z, KC_M, COMBO_END};  // Z+M: toggle zen/crafted
const uint16_t PROGMEM switch_os[] = {_04_, _28_, COMBO_END};
const uint16_t PROGMEM print_os[] = {_03_, _28_, _04_, COMBO_END};
const uint16_t PROGMEM switch_layout[] = {_07_, _31_, COMBO_END};
const uint16_t PROGMEM print_layout[] = {_08_, _31_, _07_, COMBO_END};

combo_t key_combos[] = {
    COMBO_ACTION(toggle_mode_combo),  // COMBO_TOGGLE_MODE
    COMBO_ACTION(switch_os),          // COMBO_SWITCH_OS
    COMBO_ACTION(print_os),           // COMBO_PRINT_OS
    COMBO_ACTION(switch_layout),      // COMBO_SWITCH_LAYOUT
    COMBO_ACTION(print_layout),       // COMBO_PRINT_LAYOUT
    XC_WEAK_CORNERS_COMBOS            // COMBO_WC_TL/TR/BL/BR (when XC_WEAK_CORNERS)
    COMBO(boot_combo, QK_BOOT),
};

#ifdef XC_WEAK_CORNERS
// Runtime lookup: [layer][corner] — TL=0, TR=1, BL=2, BR=3
static const uint16_t wc_keycodes[][4] = {
    [C_BASE]     = { WC_OUT_01, WC_OUT_10, WC_OUT_29, WC_OUT_30 },
#   define XC_LAYOUT_SWITCH_TARGET XC_SECONDARY_LAYOUT
#   include "feature_layout_switch.h"
    [C_BASE_ALT] = { WC_OUT_01, WC_OUT_10, WC_OUT_29, WC_OUT_30 },
};
// Restore primary _XX_ macros for keymaps[C_BASE] below
#   define XC_LAYOUT_SWITCH_TARGET XC_LAYOUT
#   include "feature_layout_switch.h"
#endif

// ============================================================================
// Key Overrides
// ============================================================================

const key_override_t* key_overrides[] = {
#ifdef XC_ALT_BASE_SYMBOLS
    ALT_SYMBOL_OVERRIDE(AS_QUOT, KC_QUOT, KC_DQUO),
    ALT_SYMBOL_OVERRIDE(AS_COMM, KC_COMM, KC_QUES),
    ALT_SYMBOL_OVERRIDE(AS_DOT,  KC_DOT,  KC_EXLM),
    ALT_SYMBOL_OVERRIDE(AS_MINS, KC_MINS, KC_SLSH),
    ALT_SYMBOL_OVERRIDE(AS_UNDS, KC_UNDS, KC_PIPE),
#endif
    SL_OVERRIDE(SL_1,    KC_1,    KC_AT),
    SL_OVERRIDE(SL_2,    KC_2,    KC_DLR),
    SL_OVERRIDE(SL_3,    KC_3,    KC_PERC),
    SL_OVERRIDE(SL_4,    KC_4,    KC_HASH),
    SL_OVERRIDE(SL_5,    KC_5,    KC_AMPR),
    SL_OVERRIDE(SL_6,    KC_6,    KC_PLUS),
    SL_OVERRIDE(SL_7,    KC_7,    KC_EQL),
    SL_OVERRIDE(SL_9,    KC_9,    KC_UNDS),
    SL_OVERRIDE(SL_0,    KC_0,    KC_GRV),
    SL_OVERRIDE(SL_BSLS, KC_BSLS, KC_CIRC),
    SL_OVERRIDE(SL_LPRN, KC_LPRN, KC_LT),
    SL_OVERRIDE(SL_RPRN, KC_RPRN, KC_GT),
    SL_OVERRIDE(SL_LBRC, KC_LCBR, KC_LBRC),
    SL_OVERRIDE(SL_RBRC, KC_RCBR, KC_RBRC),
    SL_OVERRIDE(SL_SCLN, KC_COLN, KC_SCLN),
    SL_OVERRIDE(SL_TILD, KC_PIPE, KC_TILD),
    NULL
};

// ============================================================================
// Keymaps — 10 layers
// ============================================================================

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // ========================================================================
    // Crafted layers (0-6)
    // ========================================================================

    [C_BASE] = LAYOUT_split_3x6_3(
        KC_NO,    _01_,    _02_,    _03_,    _04_,    _05_,                               _06_,    _07_,    _08_,    _09_,    _10_,    KC_NO,
        KC_TAB,  _13_,    _14_,    _15_,    _16_,    _17_,                               _18_,    _19_,    _20_,    _21_,    _22_,    KC_BSPC,
        KC_NO,   _25_,    _26_,    _27_,    _28_,    _29_,                               _30_,    _31_,    _32_,    _33_,    _34_,    KC_NO,
                                            KC_NO,   SFT_LEAD, MO(C_FAVS),               MO(C_SYMBOLS), KC_SPC,  KC_NO
    ),

#   define XC_LAYOUT_SWITCH_TARGET XC_SECONDARY_LAYOUT
#   include "feature_layout_switch.h"
    [C_BASE_ALT] = LAYOUT_split_3x6_3(
        KC_NO,   _01_,    _02_,    _03_,    _04_,    _05_,                               _06_,    _07_,    _08_,    _09_,    _10_,    KC_NO,
        KC_TAB,  _13_,    _14_,    _15_,    _16_,    _17_,                               _18_,    _19_,    _20_,    _21_,    _22_,    KC_BSPC,
        KC_NO,   _25_,    _26_,    _27_,    _28_,    _29_,                               _30_,    _31_,    _32_,    _33_,    _34_,    KC_NO,
                                            KC_NO,   SFT_LEAD, MO(C_FAVS),               MO(C_SYMBOLS), KC_SPC,  KC_NO
    ),

    [C_FAVS] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_ESC,  OS_ALT,  OS_GUI,  OS_CTRL, MM_GUICTRL, SW_WIN,                          KC_NO,   KC_NO,   KC_ENT,  KC_NO,   KC_NO,   KC_DEL,
        KC_NO,   SK_UNDO, SK_CUT,  SK_COPY, SK_PSTE, KC_NO,                              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                            KC_NO,   OS_SHFT, KC_NO,                  TO(C_NAV), OS_SHFT, KC_NO
    ),

    [C_NAV] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   SK_SALL,                            KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        TO(C_BASE), KC_NO,   KC_NO,   MO(C_NAV_FASTEST), MO(C_NAV_FASTER), KC_NO,        KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                            KC_NO,   KC_LSFT, TO(C_BASE),          TO(C_BASE), KC_LSFT, KC_NO
    ),

    [C_NAV_FASTER] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   MO(C_NAV_FASTEST),   KC_NO,   KC_NO,                  SK_WORDPRV, KC_PGDN, KC_PGUP, SK_WORDNXT, KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                            KC_NO,   KC_LSFT, TO(C_BASE),          TO(C_BASE), KC_LSFT, KC_NO
    ),

    [C_NAV_FASTEST] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              SK_LINEBEG, SK_DOCEND, SK_DOCBEG, SK_LINEEND, KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                                            KC_NO,   KC_LSFT, TO(C_BASE),          TO(C_BASE), KC_LSFT, KC_NO
    ),

    [C_SYMBOLS] = LAYOUT_split_3x6_3(
        KC_NO,     KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_NO,                              KC_NO,   KC_NO,   KC_NO,     KC_NO,    KC_NO,    KC_NO,
        TO(C_BASE),  SL_1,     SL_2,     SL_3,     SL_4,    SL_5,                             SL_6,    SL_7,    KC_8,  SL_9,     SL_0,     KC_BSPC,
        KC_NO,     KC_NO,    SL_BSLS,  SL_LPRN,  SL_LBRC, KC_NO,                              KC_NO,   SL_RBRC, SL_RPRN,  SL_SCLN,  SL_TILD,  KC_NO,
                                                  KC_NO,   KC_LSFT, KC_NO,                  QK_LAYER_LOCK,   KC_LSFT, KC_NO
    ),

    // ========================================================================
    // Zen layers (7-9)
    // ========================================================================

    [Z_BASE] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                                   KC_ESC,  KC_LGUI, MO(Z_NAV),    MO(Z_CODE), KC_SPC,  KC_ENT
    ),

    [Z_CODE] = LAYOUT_split_3x6_3(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        _______, KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_PIPE,      KC_MINS, KC_EQL,  KC_UNDS, KC_PLUS, KC_COLN, KC_DQUO,
        _______, KC_LABK, KC_RABK, KC_LPRN, KC_RPRN, KC_BSLS,      XXXXXXX, XXXXXXX, KC_COMM, KC_DOT,  KC_QUES, _______,
                                   _______, _______, _______,      _______, _______, _______
    ),

    [Z_NAV] = LAYOUT_split_3x6_3(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX, _______,
                                   _______, _______, _______,      _______, _______, _______
    )
};

// ============================================================================
// State
// ============================================================================

static bool sft_lead_held = false;
static uint16_t sft_lead_timer = 0;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

static bool sw_win_active = false;

// ============================================================================
// Key processing
// ============================================================================

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Crafted features — safe to run in zen mode (no matching keycodes)
    if (!process_dead_key(keycode, record)) return false;
    if (!process_semkey(keycode, record)) return false;

    update_swapper(&sw_win_active, KC_LGUI, KC_TAB, SW_WIN, keycode, record);

    update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
    update_oneshot(&os_alt_state, KC_LALT, OS_ALT, keycode, record);
    update_oneshot(&os_gui_state, KC_LGUI, OS_GUI, keycode, record);

    update_mod_morph_oneshot(MM_GUICTRL, keycode, record);

    switch (keycode) {
        case SFT_LEAD:
            if (record->event.pressed) {
                sft_lead_timer = timer_read();
                sft_lead_held = true;
                register_code(KC_LSFT);
            } else {
                unregister_code(KC_LSFT);
                if (sft_lead_held && timer_elapsed(sft_lead_timer) < TAPPING_TERM) {
                    leader_start();
                }
                sft_lead_held = false;
            }
            return false;
    }
    return true;
}

// ============================================================================
// Oneshot helpers
// ============================================================================

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        case KC_ESC:
            return true;
        default:
            return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case OS_SHFT:
        case OS_CTRL:
        case OS_ALT:
        case OS_GUI:
        case MM_GUICTRL:
        case KC_LSFT:
        case KC_RSFT:
        case KC_LCTL:
        case KC_RCTL:
        case KC_LALT:
        case KC_RALT:
        case KC_LGUI:
        case KC_RGUI:
        case MO(C_NAV):
        case MO(C_NAV_FASTER):
        case MO(C_NAV_FASTEST):
        case MO(C_FAVS):
        case MO(C_SYMBOLS):
        case QK_LAYER_LOCK:
        case TO(C_BASE):
            return true;
        default:
            return false;
    }
}

bool is_swapper_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case OS_SHFT:
        case KC_LSFT:
        case KC_RSFT:
            return true;
        default:
            return false;
    }
}

// ============================================================================
// Combo event handler
// ============================================================================

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case COMBO_TOGGLE_MODE:
            if (pressed) {
                zen_mode = !zen_mode;
                if (zen_mode) {
                    default_layer_set(1UL << Z_BASE);
                    layer_clear();
                    autoshift_enable();
                } else {
                    default_layer_set(1UL << C_BASE);
                    layer_clear();
                    autoshift_disable();
                }
            }
            break;
        case COMBO_SWITCH_OS:
            if (pressed && !zen_mode) {
                toggle_os_platform();
            }
            break;
        case COMBO_PRINT_OS:
            if (pressed && !zen_mode) {
                send_string(get_os_platform_name());
            }
            break;
        case COMBO_SWITCH_LAYOUT:
            if (pressed && !zen_mode) {
                if (get_highest_layer(default_layer_state) == C_BASE) {
                    default_layer_set(1UL << C_BASE_ALT);
                } else {
                    default_layer_set(1UL << C_BASE);
                }
            }
            break;
        case COMBO_PRINT_LAYOUT:
            if (pressed && !zen_mode) {
                if (get_highest_layer(default_layer_state) == C_BASE) {
                    send_string(XC_LAYOUT_NAME);
                } else {
                    send_string(XC_SECONDARY_LAYOUT_NAME);
                }
            }
            break;
#ifdef XC_WEAK_CORNERS
        case COMBO_WC_TL: case COMBO_WC_TR:
        case COMBO_WC_BL: case COMBO_WC_BR:
            if (pressed && !zen_mode) {
                uint8_t layer = get_highest_layer(default_layer_state);
                uint8_t corner = combo_index - COMBO_WC_TL;
                if (layer >= ARRAY_SIZE(wc_keycodes)) layer = C_BASE;
                tap_code(wc_keycodes[layer][corner]);
            }
            break;
#endif
    }
}

// ============================================================================
// Leader key sequences (crafted only)
// ============================================================================

void leader_end_user(void) {
    if (leader_sequence_one_key(KC_W)) {
        tap_semkey_code(SK_EURO);
    } else if (leader_sequence_one_key(KC_C)) {
        tap_semkey_code(SK_CEDIL);
    } else if (leader_sequence_one_key(KC_M)) {
        tap_semkey_code(SK_NTILDE);
    } else if (leader_sequence_one_key(KC_E)) {
        tap_deadkey_code(DK_ACUTE);
    } else if (leader_sequence_one_key(KC_A)) {
        tap_deadkey_code(DK_GRAVE);
    } else if (leader_sequence_one_key(KC_U)) {
        tap_deadkey_code(DK_DIAE);
    } else if (leader_sequence_one_key(KC_O)) {
        tap_deadkey_code(DK_CIRC);
    }
}

// ============================================================================
// Startup: disable auto-shift for crafted mode (default)
// ============================================================================

void keyboard_post_init_user(void) {
    autoshift_disable();
}
