// Copyright 2025 Xavier (@xavier)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Layer definitions
enum layers {
    BASE,     // QWERTY base layer
    CODE,     // Programming symbols and brackets
    NAV       // Navigation and function keys
};

// Combo definitions
const uint16_t PROGMEM boot_combo[] = {KC_TAB, KC_QUOT, COMBO_END};  // Tab + ' → Bootloader

combo_t key_combos[] = {
    COMBO(boot_combo, QK_BOOT),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * Base Layer (QWERTY)
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
     * │ Tab │  Q  │  W  │  E  │  R  │  T  │       │  Y  │  U  │  I  │  O  │  P  │ Bsp │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │ Ctl │  A  │  S  │  D  │  F  │  G  │       │  H  │  J  │  K  │  L  │  ;  │  '  │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │ Sft │  Z  │  X  │  C  │  V  │  B  │       │  N  │  M  │  ,  │  .  │  /  │ Sft │
     * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
     *                     ┌─────┐                           ┌─────┐
     *                     │ Esc ├─────┐               ┌─────┤ Ent │
     *                     └─────┤ GUI ├─────┐   ┌─────┤ Spc ├─────┘
     *                           └─────┤ NAV │   │CODE ├─────┘
     *                                 └─────┘   └─────┘
     */
    [BASE] = LAYOUT_split_3x6_3(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                                   KC_ESC,  KC_LGUI, MO(NAV),      MO(CODE), KC_SPC,  KC_ENT
    ),

    /*
     * CODE Layer - Programming Symbols and Numbers
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
     * │  `  │  1  │  2  │  3  │  4  │  5  │       │  6  │  7  │  8  │  9  │  0  │ Del │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │     │  {  │  }  │  [  │  ]  │  |  │       │  -  │  =  │  _  │  +  │  :  │  "  │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │     │  <  │  >  │  (  │  )  │  \  │       │     │     │  ,  │  .  │  ?  │     │
     * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
     *                     ┌─────┐                           ┌─────┐
     *                     │     ├─────┐               ┌─────┤     │
     *                     └─────┤     ├─────┐   ┌─────┤     ├─────┘
     *                           └─────┤ /// │   │ /// ├─────┘
     *                                 └─────┘   └─────┘
     */
    [CODE] = LAYOUT_split_3x6_3(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        _______, KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_PIPE,      KC_MINS, KC_EQL,  KC_UNDS, KC_PLUS, KC_COLN, KC_DQUO,
        _______, KC_LABK, KC_RABK, KC_LPRN, KC_RPRN, KC_BSLS,      XXXXXXX, XXXXXXX, KC_COMM, KC_DOT,  KC_QUES, _______,
                                   _______, _______, _______,      _______, _______, _______
    ),

    /*
     * NAV Layer - Navigation and Function Keys
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
     * │ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │       │ F7  │ F8  │ F9  │ F10 │ F11 │ F12 │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │     │     │     │     │     │     │       │ Left│ Down│ Up  │Right│     │     │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
     * │     │     │     │     │     │     │       │ Home│PgDn │PgUp │ End │     │     │
     * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
     *                     ┌─────┐                           ┌─────┐
     *                     │     ├─────┐               ┌─────┤     │
     *                     └─────┤     ├─────┐   ┌─────┤ /// ├─────┘
     *                           └─────┤     │   │     ├─────┘
     *                                 └─────┘   └─────┘
     */
    [NAV] = LAYOUT_split_3x6_3(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX, _______,
                                   _______, _______, _______,      _______, _______, _______
    )
};
