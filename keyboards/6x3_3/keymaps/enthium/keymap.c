// Copyright 2023 Kael Soares Augusto (@Dwctor)
// SPDX-License-Identifier: GPL-2.0-or-later

/*
 * Enthium keymap — a fork of `crafted` for the Enthium alpha layout.
 * https://github.com/sunaku/enthium
 *
 * Unlike crafted, this keymap hosts a SINGLE base layout, so the `_XX_`
 * position-macro indirection (the layouts headers) is dropped — the BASE keycodes are
 * written inline below. Everything else (FAVS / SYMBOLS / NAV_DEL / TABS /
 * ADJUST, compose, semantic + dead keys, OS control, the SY_* symbol set) is
 * shared verbatim with crafted.
 *
 * Enthium base layout, with the two main (alpha/symbol) clusters MIRRORED left<->right
 * (each of the 3 main rows reversed) — finger roles are preserved, the hands just trade
 * clusters:
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │  Z  │  P  │  D  │  L  │  X  │       │  =  │  U  │  O  │  Y  │  Q  │     │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │  W  │  S  │  N  │  T  │ H/⌘ │  K  │       │  -  │ E/⌘ │  A  │  I  │  C  │  B  │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │ Tab │  V  │ F/A │ G/⌘ │ M/C │  J  │       │  /  │ '/C │ ./⌘ │ ,/A │  :  │ Bsp │
 * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
 *               ┌───┐                   ┌───┐
 *               │ R ├───┐           ┌───┤Spc│
 *               └───┤Sft├───┐   ┌───┤SYM├───┘
 *                   └───┤FAV│   │Ent├───┘  Esc   R on the LEFT thumb, Esc on the right
 *                       └───┘   └───┘
 *
 * The thumb cluster is unchanged EXCEPT R<->Esc were swapped to follow the alphas
 * (R now left outer pos 36, Esc now right outer pos 41). Mod/morph furniture stays
 * position-bound, so left keys keep left-hand mods (Enthium's mod scheme is already
 * mirror-symmetric, so every letter keeps the same modifier under the opposite hand):
 *   - home-row index morph: H/⌘ (pos 16) and E/⌘ (pos 19) = GUI (Ctrl on Linux)
 *   - bottom-row mod-taps: F/A G/⌘ M/C (left) | '/C ./⌘ ,/A (right); the `'`/`.`/`,` taps
 *     get `"`/`!`/`?` from SYM_MODTAP_SHIFT (pos 31/32/33) — all have basic unshifted glyphs
 * Relocated non-alphas: Tab bottom-left (pos 24), Bspc bottom-right (pos 35); home-row
 * outers carry W (pos 12) and B (pos 23); top corners (pos 0/11) are dead.
 * BASE symbols are all custom SY_* pairs (tap → shift): =→+ (pos 6), -→_ (pos 18), /→| (pos
 * 30), '→" (pos 31), .→! (pos 32), ,→? (pos 33), :→; (pos 34). Standalone `_` and `;` are
 * reached as Shift+`-` and Shift+`:`. SYMBOLS repeats these at the same positions
 * (cross-layer consistency) and adds the other 9 pairs.
 * Sft/Spc tapped together (combo) arm Compose for accents (E/A/U/O/C/N/W, Esc cancels).
 */

#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>

// Custom keycodes (must be included before the symbol/feature headers)
#include "custom_keycodes.h"

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

// Include symbol keycodes header
#include "features/symbols.h"

// Combo indices — must match key_combos[] order exactly
enum combo_events {
    COMBO_COMPOSE,
};

const uint16_t PROGMEM compose_combo[] = {KC_LSFT, KC_SPC, COMBO_END};  // Both inner thumbs tapped together: arm Compose

combo_t key_combos[] = {
    COMBO_ACTION(compose_combo), // COMBO_COMPOSE
};

// Key Overrides — generated from SYMBOL_TABLE (one unshifted/shifted pair per
// symbol, on all layers). The table and the SY_*_SHIFTED constants it references
// live in features/symbols.h; add a symbol there, not here.
const key_override_t* key_overrides[] = {
    SYMBOL_TABLE(SYM_OVR)
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

// Snapshot the home-row index morph keycodes (H left index, E right index — mirrored)
static const uint16_t gui_morph_l = LGUI_T(KC_H);
static const uint16_t gui_morph_r = RGUI_T(KC_E);

// Mod-tap keycodes for the ' . , base positions (31/32/33), named for SYM_MODTAP_SHIFT
// (symbols.h). Must equal what the BASE keymap places there so the generated case labels
// match; the shifted glyphs (SY_*_SHIFTED) live in features/symbols.h. ' / . / , all have
// BASIC unshifted glyphs (KC_QUOT/KC_DOT/KC_COMM), so a plain mod-tap taps them cleanly and
// SYM_MODTAP_SHIFT only has to inject the shifted partner.
#define SY_QUOT_MODTAP RCTL_T(KC_QUOT)
#define SY_COMM_MODTAP RALT_T(KC_COMM)
#define SY_DOT_MODTAP  RGUI_T(KC_DOT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_split_3x6_3(
        KC_NO,   KC_Z,    KC_P,    KC_D,    KC_L,         KC_X,                           SY_EQL,  KC_U,         KC_O,    KC_Y,    KC_Q,    KC_NO,
        KC_W,    KC_S,    KC_N,    KC_T,    LGUI_T(KC_H), KC_K,                           SY_MINS, RGUI_T(KC_E), KC_A,    KC_I,    KC_C,    KC_B,
        KC_TAB,  KC_V,    LALT_T(KC_F), LGUI_T(KC_G), LCTL_T(KC_M), KC_J,                 SY_SLSH, SY_QUOT_MODTAP, SY_DOT_MODTAP, SY_COMM_MODTAP, SY_COLN, KC_BSPC,
                                            KC_R,    KC_LSFT, MO(FAVS),               LT(SYMBOLS, KC_ENT), KC_SPC,  KC_ESC
    ),
     /*
      * FAVS Layer - Favorite shortcuts and navigation (shared with crafted)
      * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
      * │   │   │   │   │   │   │       │PgU│L← │ ↑ │L→ │   │   │
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │Esc│   │Dl⊙│Tab│Sl⊙│SWn│       │PgD│ ← │ ↓ │ → │   │Del│
      * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
      * │   │Udo│Cut│Cpy│Pst│Lck│       │   │W← │   │W→ │   │   │
      * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
      * Thumbs ▽ = base Esc / Shift / Space / R (36/37/40/41)
      */
    [FAVS] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,  KC_NO,                              KC_PGUP, SK_LINEBEG, KC_UP, SK_LINEEND, KC_NO,   KC_NO,
        KC_ESC,  KC_NO,   MO(NAV_DEL), MO(TABS),  SEL_LATCH,  SW_WIN,                    KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO,   KC_DEL,
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
        KC_NO,     KC_0,     KC_1,     KC_2,     KC_3,    KC_NO,                              SY_MINS, SY_LPRN, SY_DLR,   SY_RPRN,  SY_BSLS,  SY_AMPR,
        KC_NO,     ARROW_OP, KC_4,     KC_5,     KC_6,    QK_LLCK,                            SY_SLSH, SY_QUOT, SY_DOT,   SY_COMM,  SY_COLN,  KC_NO,
                                                  _______, _______, _______,                  KC_NO,   _______, _______
    ),
     /*
      * NAV_DEL Layer - deletion sub-layer, active only while Dl⊙ is held on FAVS (shared)
      */
    [NAV_DEL] = LAYOUT_split_3x6_3(
        _______, _______, _______, _______, _______, _______,                            _______, SK_DELLINEBEG, _______, SK_DELLINEEND, _______, _______,
        _______, _______, _______, XXXXXXX, XXXXXXX, _______,                            _______, KC_BSPC, _______, KC_DEL,  _______, _______,
        _______, _______, _______, _______, _______, XXXXXXX,                            _______, SK_DELWORDPRV, _______, SK_DELWORDNXT, _______, _______,
                                            _______, _______, _______,                  _______, _______, _______
    ),
     /*
      * TABS Layer - browser tab management, active only while the trigger is held on FAVS (shared)
      */
    [TABS] = LAYOUT_split_3x6_3(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   KC_NO,    SK_TABNEW, KC_NO,     KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   _______, KC_NO,   KC_NO,                              KC_NO,   SK_TABLEFT, SK_TABCLOSE, SK_TABRIGHT, KC_NO, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                              KC_NO,   SK_HISTPRV, SK_TABREOPEN, SK_HISTNXT, KC_NO, KC_NO,
                                            _______, _______, _______,                  _______, _______, _______
    ),
     /*
      * ADJUST Layer - tri-layer: hold both inner thumbs (FAVS + SYMBOLS) (shared with crafted)
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

    // OS morph: home-row index mod-taps (E/H) use GUI on macOS, Ctrl on Linux
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

        // Custom shifted glyph for the ' . , mod-taps (pos 31/32/33); logic in symbols.h
        SYM_MODTAP_SHIFT(QUOT) // ' → "
        SYM_MODTAP_SHIFT(COMM) // , → ?
        SYM_MODTAP_SHIFT(DOT)  // . → !
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
