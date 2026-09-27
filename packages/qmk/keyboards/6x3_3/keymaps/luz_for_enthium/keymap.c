// Copyright 2023 Kael Soares Augusto (@Dwctor)
// SPDX-License-Identifier: GPL-2.0-or-later

/*
 * Luz for Enthium — the Luz conventions on the Enthium alpha layout.
 * https://github.com/sunaku/enthium
 *
 * Unlike luz_for_gallium, this keymap hosts a SINGLE base layout, so the `_XX_`
 * position-macro indirection (the layouts headers) is dropped — the BASE keycodes are
 * written inline below. Everything else (EXTEND / SYMBOLS / EXTEND_DEL / EXTEND_TABS /
 * ADJUST, compose, semantic + dead keys, OS control, the SY_* symbol set) is
 * shared verbatim with luz_for_gallium.
 *
 * Enthium base layout, with the two main (alpha/symbol) clusters MIRRORED left<->right
 * (each of the 3 main rows reversed) — finger roles are preserved, the hands just trade
 * clusters:
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │  Z  │  P  │  D  │  L  │  X  │       │  =  │  U  │  O  │  Y  │  Q  │     │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │  W  │  S  │  N  │  T  │  H  │ K/⌘ │       │ -/⌘ │  E  │  A  │  I  │  C  │  B  │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │ Tab │  V  │ F/A │ G/G │ M/C │  J  │       │  /  │ '/C │ ./G │ ,/A │  :  │ Bsp │
 * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
 *               ┌───┐                   ┌───┐
 *               │   ├───┐           ┌───┤   │
 *               └───┤ ⇧ ├───┐   ┌───┤Spc├───┘
 *                   └───┤R/F│   │SYM├───┘   R/F=tap R, hold EXTEND; SYM=SYMBOLS layer-tap
 *                       └───┘   └───┘
 *
 * The thumb cluster follows the Luz frame: 37 is the plain Shift, 39 the SYMBOLS layer-tap
 * (tap = Enter), 40 Space. Enthium's one liberty is 38, where the EXTEND hold carries R as
 * its tap, giving R the best key on the board; 36 and 41 are blank in every variant.
 * Mod/morph furniture stays position-bound, so left keys keep left-hand mods (Enthium's mod
 * scheme is already mirror-symmetric, so every letter keeps the same modifier under the
 * opposite hand):
 *   - inner-index morph: K/⌘ (pos 17) and -/⌘ (pos 18) = GUI (Ctrl on Linux). NOT the home
 *     row: 16/19 would put it under H/E, a common cross-hand bigram (see luz/mods.h).
 *   - bottom-row mod-taps: F/A G/G M/C (left) | '/C ./G ,/A (right); the `'`/`.`/`,` taps
 *     get `"`/`!`/`?` from SYM_MODTAP_SHIFT (pos 31/32/33) — all have basic unshifted glyphs
 * Relocated non-alphas: Tab bottom-left (pos 24), Bspc bottom-right (pos 35); home-row
 * outers carry W (pos 12) and B (pos 23); top corners (pos 0/11) are dead.
 * BASE symbols are all custom SY_* pairs (tap → shift): =→+ (pos 6), -→_ (pos 18), /→| (pos
 * 30), '→" (pos 31), .→! (pos 32), ,→? (pos 33), :→; (pos 34). Standalone `_` and `;` are
 * reached as Shift+`-` and Shift+`:`. SYMBOLS repeats these at the same positions
 * (cross-layer consistency) and adds the other 9 pairs.
 * Shift then Space (the thumb combo at 37+40, shared by every variant) arms Compose for
 * accents (E/A/U/O/C/N/W, Esc cancels).
 */

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>

// Custom keycodes (must be included before the symbol/feature headers)
#include "luz/custom_keycodes.h"

// Luz shared layer model (BASE + EXTEND/SYMBOLS + EXTEND_DEL/EXTEND_TABS/ADJUST)
#include "luz/layers.h"

// Inner-index morph keys (the luz/cmd_ctrl_morph module), consumed by luz/luz.h. SYMBOLS 18
// repeats `-` as a plain SY_MINS, so `-` keeps its position on both layers while the morph
// stays BASE-only.
#define LUZ_MORPH_L LGUI_T(KC_K)
#define LUZ_MORPH_R RGUI_T(KC_MINS)

// Mod-tap keycodes for the ' . , base positions (31/32/33), named for SYM_MODTAP_SHIFT
// (symbols.h). Must equal what the BASE keymap places there so the generated case labels
// match; the shifted glyphs (SY_*_SHIFTED) live in luz/symbols.h. ' / . / , all have
// BASIC unshifted glyphs (KC_QUOT/KC_DOT/KC_COMM), so a plain mod-tap taps them cleanly and
// SYM_MODTAP_SHIFT only has to inject the shifted partner.
#define SY_QUOT_MODTAP RCTL_T(KC_QUOT)
#define SY_COMM_MODTAP RALT_T(KC_COMM)
#define SY_DOT_MODTAP  RGUI_T(KC_DOT)

// BASE pos 18 doubles as the right morph: tap -, hold Cmd/Ctrl. SYMBOLS 18 repeats `-` as a
// plain SY_MINS, so `-` keeps the same position on both layers.
#define SY_MINS_MODTAP RGUI_T(KC_MINS)

// Enthium's extra custom-shift mod-taps, on top of the shared , and . (luz/luz.h)
#define LUZ_VARIANT_MODTAP_SHIFTS \
    SYM_MODTAP_SHIFT(QUOT) /* ' → " */ \
    SYM_MODTAP_SHIFT(MINS) /* - → _  (BASE pos 18, doubles as the right morph) */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * BASE — Enthium on the Luz frame
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │ Z │ P │ D │ L │ X │       │ = │ U │ O │ Y │ Q │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │ W │ S │ N │ T │ H │K/⌘│       │-/⌘│ E │ A │ I │ C │ B │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Tab│ V │F/A│G/G│M/C│ J │       │ / │'/C│./G│,/A│ : │Bsp│
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      *               ┌───┐                   ┌───┐
      *               │   ├───┐           ┌───┤   │
      *               └───┤ ⇧ ├───┐   ┌───┤Spc├───┘
      *                   └───┤R/F│   │SYM├───┘       R/F=tap R, hold EXTEND; SYM=SYMBOLS
      *                       └───┘   └───┘
      * SHIFT is a PLAIN modifier on thumb 37 — not a mod-tap at all. Thumbs are '*' (exempt)
      * in chordal_hold_layout, so one key holds for BOTH hands: the modifier you hold most
      * costs no tap-hold arbitration. Double-tap it to arm Caps Word.
      * CMD/CTRL MORPH is the mirrored pair at 17/18 (⌘ legends below) — mirrored because it
      * IS subject to the opposite-hands rule, and at 17/18 rather than the old 16/19 because
      * the index home pair spans a common CROSS-HAND bigram (th in Gallium, he in Enthium),
      * which is precisely the case Chordal Hold does not guard.
      * The home row (15/16/19/20) carries no mod-taps.
      * COMPOSE is the thumb combo Shift(37)+Space(40), in that order — see luz/compose_combo.h.
      * pos 36 and 41 are blank; Esc lives on EXTEND (12) and Enter is the tap of
      * LT(SYMBOLS) at 39, so nothing unique is lost.
      * THUMB 38 IS LT(EXTEND, KC_R) here: R gets the best thumb key, and 37 is freed for the
      * plain Shift every variant now shares. Cost — is_flow_tap_key() matches on the TAP
      * keycode, and KC_R is an alpha, so Flow Tap engages: EXTEND cannot be entered within
      * FLOW_TAP_TERM (150ms) of a keystroke. LT(SYMBOLS, KC_ENT) is exempt because KC_ENT is
      * not a flow-tap key. An is_flow_tap_key() override would remove this.
      * pos 18 is RGUI_T(KC_MINS): tap -, hold morph. SYMBOLS 18 repeats `-` as a plain
      * SY_MINS, so `-` keeps the same position on both layers and the morph stays BASE-only.
      * Bottom-row mod-taps: F/A=Alt, G/G=GUI, M/C=Ctrl | '/C=Ctrl, ./G=GUI, ,/A=Alt
      */
    [BASE] = LAYOUT_split_3x6_3(
        KC_NO,   KC_Z,    KC_P,    KC_D,    KC_L,         KC_X,                           SY_EQL,  KC_U,         KC_O,    KC_Y,    KC_Q,    KC_NO,
        KC_W,    KC_S,    KC_N,    KC_T,    KC_H,    LGUI_T(KC_K),                       SY_MINS_MODTAP, KC_E,    KC_A,    KC_I,    KC_C,    KC_B,
        KC_TAB,  KC_V,    LALT_T(KC_F), LGUI_T(KC_G), LCTL_T(KC_M), KC_J,                 SY_SLSH, SY_QUOT_MODTAP, SY_DOT_MODTAP, SY_COMM_MODTAP, SY_COLN, KC_BSPC,
                                            KC_NO,   KC_LSFT, LT(EXTEND, KC_R),         LT(SYMBOLS, KC_ENT), KC_SPC,  KC_NO
    ),
     /*
      * EXTEND Layer - Favorite shortcuts and navigation (shared with luz_for_gallium)
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │   │   │   │   │   │       │PgU│L← │ ↑ │L→ │   │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Esc│   │Dl⊙│Tab│Sel│SWn│       │PgD│ ← │ ↓ │ → │   │Del│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │Udo│Cut│Cpy│Pst│Lck│       │   │W← │   │W→ │   │   │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      * Thumbs ▽ = base Esc / Shift / Space / R (36/37/40/41)
      */
    [EXTEND] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_NO,                              KC_PGUP, SK_LINEBEG, KC_UP, SK_LINEEND, KC_NO,   KC_NO,
        KC_ESC,  KC_NO,   MO(EXTEND_DEL), MO(EXTEND_TABS),  KC_LSFT,    SW_WIN,                    KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO,   KC_DEL,
        _______, SK_UNDO, SK_CUT,  SK_COPY, SK_PSTE, QK_LLCK,                            KC_NO,   SK_WORDPRV, KC_NO, SK_WORDNXT, KC_NO,   KC_NO,
                                            _______, _______, KC_NO,                  _______, _______, _______
    ),
     /*
      * SYMBOLS Layer - numpad + macros on the left, all 16 custom SY_* pairs on the right.
      * Right-hand symbols use ONLY the custom pairs (tap shows below; Shift gives the partner).
      * The 7 symbols that also live on BASE sit at their BASE positions (cross-layer
      * consistency): = (6), - (18), / (30), ' (31), . (32), , (33), : (34). Brackets stack
      * open-over-open / close-over-close ({ over (, } over )).
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │```│ 7 │ 8 │ 9 │   │       │ = │ { │ @ │ } │ ` │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │ 0 │ 1 │ 2 │ 3 │   │       │ - │ ( │ $ │ ) │ \ │ & │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │-> │ 4 │ 5 │ 6 │Lck│       │ / │ ' │ . │ , │ : │   │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      */
    [SYMBOLS] = LAYOUT_split_3x6_3(
        KC_NO,     MD_FENCE, KC_7,     KC_8,     KC_9,    KC_NO,                              SY_EQL,  SY_LCBR, SY_AT,    SY_RCBR,  SY_GRV,   KC_NO,
        KC_NO,     KC_0,     KC_1,     KC_2,     KC_3,    KC_NO,                                SY_MINS, SY_LPRN, SY_DLR,   SY_RPRN,  SY_BSLS,  SY_AMPR,
        _______,   ARROW_OP, KC_4,     KC_5,     KC_6,    QK_LLCK,                            SY_SLSH, SY_QUOT, SY_DOT,   SY_COMM,  SY_COLN,  _______,
                                                  _______, _______, _______,                  KC_NO,   _______, _______
    ),
     /*
      * EXTEND_DEL Layer - deletion sub-layer, active only while Dl⊙ is held on EXTEND (shared)
      */
    [EXTEND_DEL] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                            _______, SK_DELLINEBEG, _______, SK_DELLINEEND, _______, _______,
        _______, _______, _______, XXXXXXX, XXXXXXX, _______,                            _______, KC_BSPC, _______, KC_DEL,  _______, _______,
        _______, _______, _______, _______, _______, XXXXXXX,                            _______, SK_DELWORDPRV, _______, SK_DELWORDNXT, _______, _______,
                                            _______, _______, _______,                  _______, _______, _______
    ),
     /*
      * EXTEND_TABS Layer - browser tab management, active only while the trigger is held on EXTEND (shared)
      */
    [EXTEND_TABS] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,    SK_TABNEW, KC_NO,     KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   _______, KC_NO,   KC_NO,                              KC_NO,   SK_TABLEFT, SK_TABCLOSE, SK_TABRIGHT, KC_NO, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   SK_HISTPRV, SK_TABREOPEN, SK_HISTNXT, KC_NO, KC_NO,
                                            _______, _______, _______,                  _______, _______, _______
    ),
     /*
      * ADJUST Layer - tri-layer: hold both inner thumbs (EXTEND + SYMBOLS) (shared with luz_for_gallium)
      */
    [ADJUST] = LAYOUT_split_3x6_3(
        KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                              KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_NO,
        QK_BOOT, SW_OS,   KC_NO,   KC_NO,   KC_NO,   KC_F11,                             KC_F12,  KC_MUTE, KC_VOLU, KC_BRIU, KC_NO,   KC_NO,
        KC_NO,   PR_OS,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_PSCR, KC_VOLD, KC_BRID, KC_NO,   KC_NO,
                                            _______, _______, _______,                  _______, _______, _______
    )
};

// Shared keymap logic (Compose, combos, key overrides, process_record_user, ...)
#include "luz/luz.h"
