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

// Custom keycodes (must be included before the base layout header)
#include "luz/custom_keycodes.h"

// Base layout: Colemak Mod-DH, matrix variant (defines the _XX_ position macros)
#include "layouts/colemak_dh.h"

// Luz shared layer model (BASE + EXTEND/SYMBOLS + EXTEND_DEL/EXTEND_TABS/ADJUST)
#include "luz/layers.h"

// Inner-index morph keys (the luz/cmd_ctrl_morph module), consumed by luz/luz.h.
#define LUZ_MORPH_L LGUI_T(_17_)
#define LUZ_MORPH_R RGUI_T(_18_)

// Mod-tap keycodes for the , / . base positions (32/33), named for SYM_MODTAP_SHIFT
// (symbols.h). Must equal what the BASE keymap places there so the generated case
// labels match; the shifted glyphs (SY_*_SHIFTED) live in luz/symbols.h.
#define SY_COMM_MODTAP RGUI_T(_32_KC)
#define SY_DOT_MODTAP  RALT_T(_33_KC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * BASE — Colemak Mod-DH (matrix variant) on the Luz frame
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │ Q │ W │ F │ P │ B │       │ J │ L │ U │ Y │ ' │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Tab│ A │ R │ S │ T │G/⌘│       │M/⌘│ N │ E │ I │ O │Bsp│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │ Z │X/A│C/G│D/C│ V │       │ K │H/C│,/G│./A│ / │ - │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      *               ┌───┐                   ┌───┐
      *               │   ├───┐           ┌───┤   │
      *               └───┤ ⇧ ├───┐   ┌───┤Spc├───┘
      *                   └───┤FAV│   │SYM├───┘       FAV=EXTEND layer, SYM=SYMBOLS layer
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
      * pos 34=SY_SLSH (/ → |), pos 35=SY_MINS (- → _). ' replaces ; at pos 10.
      * Bottom-row mod-taps: X/A=Alt, C/G=GUI, D/C=Ctrl | H/C=Ctrl, ,/G=GUI, ./A=Alt
      *
      * MOD LOAD — Colemak-DH's bottom row is z x c d v / k h , . so the Alt/GUI/Ctrl trio
      * lands on X, C, D and H: ~13.3% of English letters against ~6.8% for Gallium East.
      */
    [BASE] = LAYOUT_split_3x6_3(
        KC_NO,    _01_,    _02_,    _03_,    _04_,    _05_,                               _06_,    _07_,    _08_,    _09_,    _10_,    KC_NO,
        KC_TAB,  _13_,    _14_,    _15_,    _16_,    LGUI_T(_17_),                       RGUI_T(_18_), _19_,    _20_,    _21_,    _22_,    KC_BSPC,
        KC_NO,   _25_,    LALT_T(_26_), LGUI_T(_27_), LCTL_T(_28_), _29_,               _30_,    RCTL_T(_31_), RGUI_T(_32_KC), RALT_T(_33_KC), _34_, _35_,
                                            KC_NO,   KC_LSFT, MO(EXTEND),               LT(SYMBOLS, KC_ENT), KC_SPC,  KC_NO
    ),
     /*
      * EXTEND Layer - Favorite shortcuts and navigation
      * WASD-style inverted-T arrows; magnitude grows away from home row (line above, word below)
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │   │   │   │   │   │       │PgU│L← │ ↑ │L→ │   │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Esc│   │Dl⊙│Tab│Sel│SWn│       │PgD│ ← │ ↓ │ → │   │Del│
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
      * Lck=Layer Lock (keep EXTEND without holding the thumb)
      * Sel=Select: hold (plain Shift) so the right-hand motions select instead of move
      * Dl⊙=Delete hold: momentary EXTEND_DEL sub-layer (hold-only, destructive op)
      * Tab=Tab mode: momentary EXTEND_TABS sub-layer (hold-only) — browser tab management
      * L←=Line Begin, L→=Line End, W←=Word Left, W→=Word Right
      * PgU/PgD=vertical pair on inner column (doc begin/end dropped)
      */
    [EXTEND] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_NO,                              KC_PGUP, SK_LINEBEG, KC_UP, SK_LINEEND, KC_NO,   KC_NO,
        KC_ESC,  KC_NO,   MO(EXTEND_DEL), MO(EXTEND_TABS),  KC_LSFT,    SW_WIN,                    KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO,   KC_DEL,
        _______, SK_UNDO, SK_CUT,  SK_COPY, SK_PSTE, QK_LLCK,                            KC_NO,   SK_WORDPRV, KC_NO, SK_WORDNXT, KC_NO,   KC_NO,
                                            _______, _______, KC_NO,                  _______, _______, _______
    ),
     /*
      * SYMBOLS: numpad on the left, symbol field on the right
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │```│ 7 │ 8 │ 9 │   │       │ ` │ { │ = │ } │ ' │   │
      * │   │   │   │   │   │   │       │ ~ │ [ │ + │ ] │ " │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │ ▽ │ 0 │ 1 │ 2 │ 3 │   │       │ \ │ ( │ @ │ ) │ : │ ▽ │
      * │   │   │   │   │   │   │       │ ^ │ < │ # │ > │ ; │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │ ▽ │→ ⇒│ 4 │ 5 │ 6 │Lck│       │ $ │ & │ , │ . │ / │ ▽ │
      * │   │   │   │   │   │   │       │ % │ * │ ? │ ! │ | │   │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      *               ┌───┐                   ┌───┐
      *               │ ▽ ├───┐           ┌───┤ ▽ │
      *               └───┤ ▽ ├───┐   ┌───┤ ▽ ├───┘
      *                   └───┤   │   │(M)├───┘
      *                       └───┘   └───┘
      * Left hand is the shared Luz numpad, identical to every other variant.
      * Right hand is this variant's placement: open brackets on the index column, close on
      * ring, pairs stacked by kind ({[ over (<, }] over )>), middle finger keeps =/@.
      * ' , . / sit on their BASE positions (cross-layer consistency); `-` is reached at 35
      * by fall-through to BASE, the same trick Gallium uses for `/`.
      * ```=code fence macro, →⇒=tap "->" / shift "=>"
      * Lck=Layer Lock; (M)=held MO(SYMBOLS) thumb
      * ▽=fall-through to base (Tab at 12, SY_MINS at 35, Bspc at 23, thumbs Esc/Sft/Spc/Ent)
      */
    [SYMBOLS] = LAYOUT_split_3x6_3(
        KC_NO,     MD_FENCE, KC_7,     KC_8,     KC_9,    KC_NO,                              SY_GRV,  SY_LCBR, SY_EQL,   SY_RCBR,  SY_QUOT,  KC_NO,
        _______,   KC_0,     KC_1,     KC_2,     KC_3,    KC_NO,                                SY_BSLS, SY_LPRN, SY_AT,    SY_RPRN,  SY_COLN,  _______,
        _______,   ARROW_OP, KC_4,     KC_5,     KC_6,    QK_LLCK,                            SY_DLR,  SY_AMPR, SY_COMM,  SY_DOT,   SY_SLSH,  _______,
                                                  _______, _______, _______,                  KC_NO,   _______, _______
    ),
     /*
      * EXTEND_DEL Layer (Layer 4) - Deletion sub-layer, active only while Dl⊙ is held on EXTEND
      * Vim-like operator grammar: row = granularity, each deletion sits on the motion it consumes
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │   │   │   │   │   │       │ ▽ │DlB│ ▽ │DlE│   │   │  line: delete to begin/end
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │ ▽ │ ▽ │(▽)│ ✗ │ ✗ │ ▽ │       │ ▽ │Bsp│ ▽ │Del│   │ ▽ │  char: backspace/delete
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │ ▽ │ ▽ │ ▽ │ ▽ │ ✗ │       │   │DlW│ ▽ │Dl→│   │   │  word: delete back/forward
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      * ▽=transparent (EXTEND motions/clipboard stay live: navigate, page, Undo without releasing)
      * ✗=blocked: Lck (lock delete mode), Tab (tab mode), Sel (delete wins over select)
      * (▽)=Dl⊙ itself (the held MO key)
      */
    [EXTEND_DEL] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                            _______, SK_DELLINEBEG, _______, SK_DELLINEEND, _______, _______,
        _______, _______, _______, XXXXXXX, XXXXXXX, _______,                            _______, KC_BSPC, _______, KC_DEL,  _______, _______,
        _______, _______, _______, _______, _______, XXXXXXX,                            _______, SK_DELWORDPRV, _______, SK_DELWORDNXT, _______, _______,
                                            _______, _______, _______,                  _______, _______, _______
    ),
     /*
      * EXTEND_TABS Layer - Browser tab management, active only while the trigger (pos 15) is held on EXTEND
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
    [EXTEND_TABS] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,    SK_TABNEW, KC_NO,     KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   _______, KC_NO,   KC_NO,                              KC_NO,   SK_TABLEFT, SK_TABCLOSE, SK_TABRIGHT, KC_NO, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   SK_HISTPRV, SK_TABREOPEN, SK_HISTNXT, KC_NO, KC_NO,
                                            _______, _______, _______,                  _______, _______, _______
    ),
     /*
      * ADJUST Layer (Layer 6) - tri-layer: hold both inner thumbs (EXTEND + SYMBOLS)
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

// Shared keymap logic (Compose, combos, key overrides, process_record_user, ...)
#include "luz/luz.h"
