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

// Base layout: QWERTY (defines the _XX_ position macros)
#include "layouts/qwerty.h"

// Callum-style swapper
#include "luz/swapper.h"

// OS control for platform-aware features
#include "luz/os_control.h"

// Luz shared layer model (BASE + EXTEND/SYMBOLS + EXTEND_DEL/EXTEND_TABS/ADJUST)
#include "luz/layers.h"

// Luz shared Compose combo — Shift(37)+Space(40), identical in every variant
#include "luz/compose.h"

// Include semantic keys header
#include "luz/semantic_keys.h"

// Include dead keys header
#include "luz/dead_keys.h"

// Include symbol keycodes header
#include "luz/symbols.h"

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

// Snapshot the morph keycodes for LUZ_MORPH_KEY (preprocessor expands _17_/_18_ here).
static const uint16_t morph_l = LGUI_T(_17_);
static const uint16_t morph_r = RGUI_T(_18_);

// Mod-tap keycodes for the , / . base positions (32/33), named for SYM_MODTAP_SHIFT
// (symbols.h). Must equal what the BASE keymap places there so the generated case
// labels match; the shifted glyphs (SY_*_SHIFTED) live in luz/symbols.h.
#define SY_COMM_MODTAP RGUI_T(_32_KC)
#define SY_DOT_MODTAP  RALT_T(_33_KC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     /*
      * BASE — QWERTY on the Luz frame
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │ Q │ W │ E │ R │ T │       │ Y │ U │ I │ O │ P │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Tab│ A │ S │ D │ F │G/⌘│       │H/⌘│ J │ K │ L │ ' │Bsp│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │ Z │X/A│C/G│V/C│ B │       │ N │M/C│,/G│./A│ / │ - │
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
      * COMPOSE is the thumb combo Shift(37)+Space(40), in that order — see luz/compose.h.
      * pos 36 and 41 are blank; Esc lives on EXTEND (12) and Enter is the tap of
      * LT(SYMBOLS) at 39, so nothing unique is lost.
      * pos 34=SY_SLSH (/ → |), pos 35=SY_MINS (- → _). ' replaces ; at pos 22 — QWERTY's
      * semicolon key is on the HOME row, not the top row as in Colemak-DH.
      * Bottom-row mod-taps: X/A=Alt, C/G=GUI, V/C=Ctrl | M/C=Ctrl, ,/G=GUI, ./A=Alt
      *
      * MORPH LOAD — the one place QWERTY fits the frame badly. 17/18 is G and H, so the
      * morph carries ~8.1% of English letters (G 2.0 + H 6.1) against ~4.0% in Gallium
      * (G/Y) and ~4.4% in Colemak-DH (G/M). H alone is more than either variant's whole
      * pair. `gh` is also a real cross-hand bigram (night, through) and Chordal Hold does
      * not guard cross-hand — though Flow Tap does, since both are alphas and `gh` is
      * effectively never word-initial. The bottom row, by contrast, is fine: X+C+V+M is
      * ~6.3%, in line with Gallium's ~6.8%. See the README.
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
      * │   │```│ 7 │ 8 │ 9 │   │       │ ` │ { │ = │ } │ : │   │
      * │   │   │   │   │   │   │       │ ~ │ [ │ + │ ] │ ; │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │ ▽ │ 0 │ 1 │ 2 │ 3 │   │       │ \ │ ( │ @ │ ) │ ' │ ▽ │
      * │   │   │   │   │   │   │       │ ^ │ < │ # │ > │ " │   │
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
      * ' AND : ARE SWAPPED relative to Gallium and Colemak-DH (' at 22, : at 10). Not taste:
      * QWERTY's apostrophe is on the HOME row, and cross-layer consistency says a symbol on
      * both BASE and SYMBOLS keeps its position — so ' takes 22 here and : takes the slot it
      * vacates. This is the whole cost of the layout change on this layer.
      * ```=code fence macro, →⇒=tap "->" / shift "=>"
      * Lck=Layer Lock; (M)=held MO(SYMBOLS) thumb
      * ▽=fall-through to base (Tab at 12, SY_MINS at 35, Bspc at 23, thumbs Esc/Sft/Spc/Ent)
      */
    [SYMBOLS] = LAYOUT_split_3x6_3(
        KC_NO,     MD_FENCE, KC_7,     KC_8,     KC_9,    KC_NO,                              SY_GRV,  SY_LCBR, SY_EQL,   SY_RCBR,  SY_COLN,  KC_NO,
        _______,   KC_0,     KC_1,     KC_2,     KC_3,    KC_NO,                                SY_BSLS, SY_LPRN, SY_AT,    SY_RPRN,  SY_QUOT,  _______,
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

    // OS morph: home-row index mod-taps (positions 16/19) use GUI on macOS, Ctrl on Linux
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

        // Custom shifted glyph for the , / . mod-taps (pos 32/33); logic in symbols.h
        SYM_MODTAP_SHIFT(COMM) // , → ?
        SYM_MODTAP_SHIFT(DOT)  // . → !
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

void suspend_power_down_user(void) {
    // code will run multiple times while keyboard is suspended
}

void suspend_wakeup_init_user(void) {
    // code will run on keyboard wakeup
}
