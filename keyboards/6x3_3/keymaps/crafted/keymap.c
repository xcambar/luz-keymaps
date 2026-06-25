// Copyright 2023 Kael Soares Augusto (@Dwctor)
// SPDX-License-Identifier: GPL-2.0-or-later

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

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>

// Custom keycodes (must be included before feature_base_layer.h)
#include "custom_keycodes.h"

// Alt-symbol pairs used directly in keymaps[] (plain keycodes when alt symbols are off)
#ifdef XC_ALT_BASE_SYMBOLS
#    define XC_UNDS AS_UNDS
#    define XC_QUOT AS_QUOT
#    define XC_COMM AS_COMM
#    define XC_DOT  AS_DOT
#    define XC_MINS AS_MINS
#else
#    define XC_UNDS KC_UNDS
#    define XC_QUOT KC_QUOT
#    define XC_COMM KC_COMM
#    define XC_DOT  KC_DOT
#    define XC_MINS KC_MINS
#endif

// Base layer switch
#include "feature_base_layer.h"

// Weak corners feature
#include "feature_weak_corners.h"

// Callum-style swapper
#include "features/swapper.h"

// OS control for platform-aware features
#include "features/os_control.h"

enum layers {
    BASE = 0,
    FAVS,
    SYMBOLS,
    NAV_DEL,
    TABS,
    ADJUST
};

// Include semantic keys header
#include "features/semantic_keys.h"

// Include dead keys header
#include "features/dead_keys.h"

// Include alternative symbols header
#include "features/alt_symbols.h"
#include "features/alt_symbols_layer.h"

// Combo indices — must match key_combos[] order exactly
enum combo_events {
    COMBO_COMPOSE,
#ifdef XC_WEAK_CORNERS
    COMBO_WC_TL,
    COMBO_WC_TR,
    COMBO_WC_BL,
    COMBO_WC_BR,
#endif
};

const uint16_t PROGMEM compose_combo[] = {KC_LSFT, KC_SPC, COMBO_END};  // Both inner thumbs tapped together: arm Compose

combo_t key_combos[] = {
    COMBO_ACTION(compose_combo), // COMBO_COMPOSE
    XC_WEAK_CORNERS_COMBOS       // COMBO_WC_TL/TR/BL/BR (when XC_WEAK_CORNERS)
};

#ifdef XC_WEAK_CORNERS
// Runtime lookup: [layer][corner] — TL=0, TR=1, BL=2, BR=3
static const uint16_t wc_keycodes[][4] = {
    [BASE] = { WC_OUT_01, WC_OUT_10, WC_OUT_29, WC_OUT_30 },
};
#endif

// Key Overrides for alternative base symbols (custom keycodes)

const key_override_t* key_overrides[] = {
#ifdef XC_ALT_BASE_SYMBOLS
    ALT_SYMBOL_OVERRIDE(AS_QUOT, KC_QUOT, KC_DQUO), // ' → "
    // Plain AS_COMM/AS_DOT (used on SYMBOLS); base-layer mod-tap versions handled in process_record_user
    ALT_SYMBOL_OVERRIDE(AS_COMM, KC_COMM, KC_QUES), // , → ?
    ALT_SYMBOL_OVERRIDE(AS_DOT,  KC_DOT,  KC_EXLM), // . → !
    ALT_SYMBOL_OVERRIDE(AS_MINS, KC_MINS, KC_SLSH), // - → /
    ALT_SYMBOL_OVERRIDE(AS_UNDS, KC_UNDS, KC_PIPE), // _ → |
#endif
    // Shifted pairs on the SYMBOLS layer
    SL_OVERRIDE(SL_AT,   KC_AT,   KC_HASH), // @ → #
    SL_OVERRIDE(SL_GRV,  KC_GRV,  KC_TILD), // ` → ~
    SL_OVERRIDE(SL_EQL,  KC_EQL,  KC_PLUS), // = → +
    SL_OVERRIDE(SL_DLR,  KC_DLR,  KC_PERC), // $ → %
    SL_OVERRIDE(SL_AMPR, KC_AMPR, KC_ASTR), // & → *
    SL_OVERRIDE(SL_BSLS, KC_BSLS, KC_CIRC), // \ → ^
    SL_OVERRIDE(SL_LPRN, KC_LPRN, KC_LT),   // ( → <
    SL_OVERRIDE(SL_RPRN, KC_RPRN, KC_GT),   // ) → >
    SL_OVERRIDE(SL_LBRC, KC_LCBR, KC_LBRC), // { → [ (inverted)
    SL_OVERRIDE(SL_RBRC, KC_RCBR, KC_RBRC), // } → ] (inverted)
    SL_OVERRIDE(SL_SCLN, KC_COLN, KC_SCLN), // : → ; (inverted)
    NULL
};

// Chordal Hold handedness: 'L'=left, 'R'=right, '*'=exempt (thumbs)
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_split_3x6_3(
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
                       '*', '*', '*',  '*', '*', '*'
    );

// Snapshot the home-row index morph keycodes (preprocessor expands _16_/_19_ here)
static const uint16_t gui_morph_l = LGUI_T(_16_);
static const uint16_t gui_morph_r = RGUI_T(_19_);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │[Q]│ W[Q]E │ R │ T │       │ Y │ U | I[P]O │[P]│   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Tab│ A │ S │ D │F/⌘│ G │       │ H │J/⌘│ K │ L │ ; │Bsp│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │ Z │X/A│C/G│V/C│[B]│       │[N]│M/C│,/G│./A│ / │ _ │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      *               ┌───┐                   ┌───┐
      *               │Esc├───┐           ┌───┤Ent│
      *               └───┤Sft├───┐   ┌───┤Spc├───┘
      *                   └───┤FAV│   │SYM├───┘       FAV=FAVS layer, SYM=SYMBOLS layer
      *                       └───┘   └───┘
      * pos 24 empty (Caps Word now via double-tap Shift), _=XC_UNDS (_ → | shifted), Esc/Ent on outer thumbs
      * Esc/Ent fall through on FAVS and SYMBOLS (transparent thumbs at 36/41); SYMBOLS
      * also inherits _ at 35, but FAVS 35 is dead (KC_NO) — nav layer doesn't want it
      * Sft/Spc are plain keys; tapping both together (combo) arms Compose for accents:
      * E/A/U/O = acute/grave/diaeresis/circumflex dead key, C=ç, N=ñ, W=€, Esc cancels,
      * any other key passes through unchanged
      * Home-row mod-taps: F/⌘=GUI (Ctrl on Linux), J/⌘=GUI (Ctrl on Linux)
      * Bottom-row mod-taps: X/A=Alt, C/G=GUI, V/C=Ctrl | M/C=Ctrl, ,/G=GUI, ./A=Alt
      * Chordal Hold: opposite-hands rule prevents same-hand roll misfires
      * Weak corners: [Q] [P] [B] [N] - only when XC_WEAK_CORNERS enabled, else actual keys
      * Combos: W+E→Q I+O→P C+V→B M+,→N (when weak corners on); boot lives on ADJUST
      */
    [BASE] = LAYOUT_split_3x6_3(
        KC_NO,    _01_,    _02_,    _03_,    _04_,    _05_,                               _06_,    _07_,    _08_,    _09_,    _10_,    KC_NO,
        KC_TAB,  _13_,    _14_,    _15_,    LGUI_T(_16_), _17_,                           _18_,    RGUI_T(_19_), _20_,    _21_,    _22_,    KC_BSPC,
        KC_NO,   _25_,    LALT_T(_26_), LGUI_T(_27_), LCTL_T(_28_), _29_,               _30_,    RCTL_T(_31_), RGUI_T(_32_KC), RALT_T(_33_KC), _34_, XC_UNDS,
                                            KC_ESC,  KC_LSFT, MO(FAVS),               LT(SYMBOLS, KC_ENT), KC_SPC,  KC_ENT
    ),
     /*
      * FAVS Layer - Favorite shortcuts and navigation
      * WASD-style inverted-T arrows; magnitude grows away from home row (line above, word below)
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │   │   │   │   │   │       │PgU│L← │ ↑ │L→ │   │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Esc│   │Dl⊙│Tab│Sl⊙│SWn│       │PgD│ ← │ ↓ │ → │   │Del│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │Udo│Cut│Cpy│Pst│Lck│       │   │W← │   │W→ │   │   │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      *               ┌───┐                   ┌───┐
      *               │ ▽ ├───┐           ┌───┤ ▽ │
      *               └───┤ ▽ ├───┐   ┌───┤ ▽ ├───┘
      *                   └───┤   │   │   ├───┘
      *                       └───┘   └───┘
      * Thumbs ▽ = base Esc / Shift / Space / Enter (36/37/40/41)
      * SWn=Switch Window
      * Lck=Layer Lock (keep FAVS without holding the thumb)
      * Sl⊙=Select latch: tap to hold Shift until FAVS is released (or tap again/Esc)
      * Dl⊙=Delete hold: momentary NAV_DEL sub-layer (hold-only, destructive op)
      * Tab=Tab mode: momentary TABS sub-layer (hold-only) — browser tab management
      * L←=Line Begin, L→=Line End, W←=Word Left, W→=Word Right
      * PgU/PgD=vertical pair on inner column (doc begin/end dropped)
      */
    [FAVS] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_NO,                              KC_PGUP, SK_LINEBEG, KC_UP, SK_LINEEND, KC_NO,   KC_NO,
        KC_ESC,  KC_NO,   MO(NAV_DEL), MO(TABS),  SEL_LATCH,  SW_WIN,                    KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO,   KC_DEL,
        _______, SK_UNDO, SK_CUT,  SK_COPY, SK_PSTE, QK_LLCK,                            KC_NO,   SK_WORDPRV, KC_NO, SK_WORDNXT, KC_NO,   KC_NO,
                                            _______, _______, KC_NO,                  _______, _______, _______
    ),
     /*
      * Layer 3 - SYMBOLS: numpad on the left, symbol field on the right
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │```│ 7 │ 8 │ 9 │   │       │ ` │ { │ = │ } │ ' │   │
      * │   │   │   │   │   │   │       │ ~ │ [ │ + │ ] │ " │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │ ▽ │ 0 │ 1 │ 2 │ 3 │   │       │ \ │ ( │ @ │ ) │ : │ ▽ │
      * │   │   │   │   │   │   │       │ ^ │ < │ # │ > │ ; │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │ ▽ │→ ⇒│ 4 │ 5 │ 6 │Lck│       │ $ │ & │ , │ . │ - │ ▽ │
      * │   │   │   │   │   │   │       │ % │ * │ ? │ ! │ / │   │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      *               ┌───┐                   ┌───┐
      *               │ ▽ ├───┐           ┌───┤ ▽ │
      *               └───┤ ▽ ├───┐   ┌───┤ ▽ ├───┘
      *                   └───┤   │   │(M)├───┘
      *                       └───┘   └───┘
      * Numpad: home row 0-3 (0 on ring), 4-6 below, 7-9 on top; open brackets on index, close on ring (editors auto-close),
      * middle finger keeps high-frequency =/@; pairs stacked by kind ({[ over (<, }] over )>)
      * , . - ' _ sit on their BASE positions (cross-layer consistency); inverted pairs kept
      * ```=code fence macro, →⇒=tap "->" / shift "=>"
      * Lck=Layer Lock: tap to lock (then release MO; 40 ▽ then gives Space), tap again to unlock
      * (M)=held MO(SYMBOLS) thumb, ▽=fall-through to base (Tab, XC_UNDS _/|, Bspc at 23,
      * and thumbs Esc/Shift/Space/Ent — same pattern as FAVS)
      */
    [SYMBOLS] = LAYOUT_split_3x6_3(
        KC_NO,     MD_FENCE, KC_7,     KC_8,     KC_9,    KC_NO,                              SL_GRV,  SL_LBRC, SL_EQL,   SL_RBRC,  XC_QUOT,  KC_NO,
        _______,   KC_0,     KC_1,     KC_2,     KC_3,    KC_NO,                              SL_BSLS, SL_LPRN, SL_AT,    SL_RPRN,  SL_SCLN,  _______,
        _______,   ARROW_OP, KC_4,     KC_5,     KC_6,    QK_LLCK,                            SL_DLR,  SL_AMPR, XC_COMM,  XC_DOT,   XC_MINS,  _______,
                                                  _______, _______, _______,                  KC_NO,   _______, _______
    ),
     /*
      * NAV_DEL Layer (Layer 4) - Deletion sub-layer, active only while Dl⊙ is held on FAVS
      * Vim-like operator grammar: row = granularity, each deletion sits on the motion it consumes
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │   │   │   │   │   │       │ ▽ │DlB│ ▽ │DlE│   │   │  line: delete to begin/end
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │ ▽ │ ▽ │(▽)│ ✗ │ ✗ │ ▽ │       │ ▽ │Bsp│ ▽ │Del│   │ ▽ │  char: backspace/delete
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │ ▽ │ ▽ │ ▽ │ ▽ │ ✗ │       │   │DlW│ ▽ │Dl→│   │   │  word: delete back/forward
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      * ▽=transparent (FAVS motions/clipboard stay live: navigate, page, Undo without releasing)
      * ✗=blocked: Lck (lock delete mode), Tab (tab mode), Sl⊙ (delete wins over select)
      * (▽)=Dl⊙ itself (the held MO key)
      */
    [NAV_DEL] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                            _______, SK_DELLINEBEG, _______, SK_DELLINEEND, _______, _______,
        _______, _______, _______, XXXXXXX, XXXXXXX, _______,                            _______, KC_BSPC, _______, KC_DEL,  _______, _______,
        _______, _______, _______, _______, _______, XXXXXXX,                            _______, SK_DELWORDPRV, _______, SK_DELWORDNXT, _______, _______,
                                            _______, _______, _______,                  _______, _______, _______
    ),
     /*
      * TABS Layer - Browser tab management, active only while the trigger (pos 15) is held on FAVS
      * Inverted-T reusing the cursor cluster: index column = tab lifecycle, home-row arms = switch
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │   │   │   │   │   │       │   │   │New│   │   │   │  New=Ctrl/Cmd+T
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │   │   │(▽)│   │   │       │   │ ◀ │Cls│ ▶ │   │   │  ◀/▶=switch tab, Cls=close
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │   │   │   │   │   │       │   │ ◅ │Rop│ ▻ │   │   │  ◅/▻=page back/forward, Rop=reopen
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      * (▽)=trigger itself (the held MO key); thumbs ▽ keep Esc/Shift/Space/Enter live.
      * Switch row over history row: both horizontal pairs flank the central Close/Reopen column.
      * All chords are OS-aware semantic keys (Linux Ctrl/Alt / macOS Cmd, Chrome/Safari positional switch;
      * history back/forward = Cmd+[ ] / Alt+arrows, identical in Firefox & Chrome).
      */
    [TABS] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,    SK_TABNEW, KC_NO,     KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   _______, KC_NO,   KC_NO,                              KC_NO,   SK_TABLEFT, SK_TABCLOSE, SK_TABRIGHT, KC_NO, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   SK_HISTPRV, SK_TABREOPEN, SK_HISTNXT, KC_NO, KC_NO,
                                            _______, _______, _______,                  _______, _______, _______
    ),
     /*
      * ADJUST Layer (Layer 6) - tri-layer: hold both inner thumbs (FAVS + SYMBOLS)
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │F1 │F2 │F3 │F4 │F5 │       │F6 │F7 │F8 │F9 │F10│   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Bot│OS⇄│   │   │   │F11│       │F12│Mut│Vl↑│Br↑│Ly⇄│   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │OS?│   │   │   │   │       │   │Scr│Vl↓│Br↓│Ly?│   │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      * Fn keys on the top row (F1-F10), F11/F12 continue on the inner home columns
      * Bot=QK_BOOT at the Tab position (sole bootloader access; the BASE combo was removed)
      * Volume (middle col) and Brightness (ring col) as vertical pairs: up on home, down below
      * OS⇄/OS?=toggle/print OS (left, as the old combos); Scr=PrtScr (Linux;
      * macOS screenshots stay on Cmd+Shift+3/4); Mut=Mute
      * pos 23 = KC_NO (was Bspc via SYMBOLS fall-through; SYMBOLS 23 is now ▽ so the
      * dependency was removed); thumbs ▽ as everywhere (Esc/Shift/Space/Ent)
      */
    [ADJUST] = LAYOUT_split_3x6_3(
        KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_NO,
        QK_BOOT, SW_OS,   KC_NO,   KC_NO,   KC_NO,   KC_F11,                             KC_F12,  KC_MUTE, KC_VOLU, KC_BRIU, KC_NO,   KC_NO,
        KC_NO,   PR_OS,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_PSCR, KC_VOLD, KC_BRID, KC_NO,   KC_NO,
                                            _______, _______, _______,                  _______, _______, _______
    )
};

// Compose state: armed by the Shift+Space thumb combo, consumed by the next keypress
static bool compose_pending = false;

// Swapper state
static bool sw_win_active = false;

// Select latch state: real Shift, scoped to the FAVS layer
static bool sel_latch_active = false;

static void sel_latch_off(void) {
    if (sel_latch_active) {
        unregister_code(KC_LSFT);
        sel_latch_active = false;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // ADJUST tri-layer: active while both FAVS and SYMBOLS are held
    state = update_tri_layer_state(state, FAVS, SYMBOLS, ADJUST);

    // Latch lifecycle: released on leaving FAVS; delete hold (NAV_DEL) wins over select
    if (!layer_state_cmp(state, FAVS) || layer_state_cmp(state, NAV_DEL)) {
        sel_latch_off();
    }
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
                sel_latch_off();
                return false;  // cancel
            case KC_E: compose_pending = false; tap_deadkey_code(DK_ACUTE); return false;
            case KC_A: compose_pending = false; tap_deadkey_code(DK_GRAVE); return false;
            case KC_U: compose_pending = false; tap_deadkey_code(DK_DIAE);  return false;
            case KC_O: compose_pending = false; tap_deadkey_code(DK_CIRC);  return false;
            case KC_C: compose_pending = false; tap_semkey_code(SK_CEDIL);  return false;
            case KC_N: compose_pending = false; tap_semkey_code(SK_NTILDE); return false;
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

    // OS morph: home-row index mod-taps (positions 16/19) use GUI on macOS, Ctrl on Linux
    // On macOS, LGUI_T/RGUI_T hold behavior is correct as-is; on Linux, swap to Ctrl
    if (get_os_platform() != OS_MacOS && !record->tap.count) {
        if (keycode == gui_morph_l) {
            if (record->event.pressed) register_code(KC_LCTL); else unregister_code(KC_LCTL);
            return false;
        }
        if (keycode == gui_morph_r) {
            if (record->event.pressed) register_code(KC_RCTL); else unregister_code(KC_RCTL);
            return false;
        }
    }

    switch (keycode) {
        case SEL_LATCH:
            if (record->event.pressed) {
                if (sel_latch_active) {
                    sel_latch_off();
                } else {
                    register_code(KC_LSFT);
                    sel_latch_active = true;
                }
            }
            return false;

        case KC_ESC:
            // Esc bails out of an active selection latch (mirrors oneshot cancel)
            if (record->event.pressed) {
                sel_latch_off();
            }
            break;

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

#ifdef XC_ALT_BASE_SYMBOLS
        // Alt-symbol shifted behavior for mod-tap keys (positions 32-33)
        // Mod-tap uses basic keycodes; custom shift handled here instead of key overrides
        case RGUI_T(KC_COMM):  // , → ? when shifted
        case RALT_T(KC_DOT):   // . → ! when shifted
            if (record->tap.count && record->event.pressed) {
                uint8_t mods = get_mods() | get_oneshot_mods();
                if (mods & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    tap_code16(keycode == RGUI_T(KC_COMM) ? KC_QUES : KC_EXLM);
                    register_mods(mods & MOD_MASK_SHIFT);
                    return false;
                }
            }
            break;
#endif
    }
    return true;
}

// Define keys that should be ignored by swapper (allows changing direction)
bool is_swapper_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case SEL_LATCH:
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
#ifdef XC_WEAK_CORNERS
        case COMBO_WC_TL: case COMBO_WC_TR:
        case COMBO_WC_BL: case COMBO_WC_BR:
            if (pressed) {
                uint8_t layer = get_highest_layer(default_layer_state);
                uint8_t corner = combo_index - COMBO_WC_TL;
                if (layer >= ARRAY_SIZE(wc_keycodes)) layer = BASE;
                uint16_t kc = wc_keycodes[layer][corner];
            }
            break;
#endif
    }
}

// Caps Word: QMK default plus AS_UNDS so SCREAMING_SNAKE_CASE survives the BASE
// alt-symbol underscore (a custom keycode the default would treat as word-breaking).
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Continue Caps Word, applying shift so the letter is uppercased:
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT));
            return true;
        // Continue Caps Word without shifting:
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
#ifdef XC_ALT_BASE_SYMBOLS
        case AS_UNDS:
#endif
            return true;
        default:
            return false;  // any other key ends Caps Word
    }
}
