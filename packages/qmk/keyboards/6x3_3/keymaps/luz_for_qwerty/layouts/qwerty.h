// QWERTY Layout Key Definitions
// This file maps abstract key positions (_XX_) to actual QWERTY keycodes.
//
// QWERTY needs no citation. It is here as the framework's hardest test: Luz was designed
// around layouts chosen for their letter placement, and QWERTY was not designed at all.
// If the contract holds on QWERTY it holds anywhere.

/*
 * QWERTY - Actual Keycodes
 *
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │  Q  │  W  │  E  │  R  │  T  │       │  Y  │  U  │  I  │  O  │  P  │     │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │     │  A  │  S  │  D  │  F  │  G  │       │  H  │  J  │  K  │  L  │  '  │     │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │     │  Z  │  X  │  C  │  V  │  B  │       │  N  │  M  │  ,  │  .  │  /  │  -  │
 * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
 *                     ┌─────┐                           ┌─────┐
 *                     │     ├─────┐               ┌─────┤     │
 *                     └─────┤     ├─────┐   ┌─────┤     ├─────┘
 *                           └─────┤     │   │     ├─────┘
 *                                 └─────┘   └─────┘
 *
 * Apostrophe replaces Semicolon at pos 22, the usual 30-key convention (Miryoku and the
 * Colemak-DH variant do the same, though there the key falls at pos 10); `;` stays reachable
 * as SY_COLN's shifted glyph on SYMBOLS.
 *
 * Privileged BASE symbols (Luz principle 1) are ' , . / - — the same five as Gallium East
 * and Colemak-DH, and at the same positions as Colemak-DH for , . / -. Only `'` differs:
 * QWERTY's semicolon key is on the HOME row (22), not the top row (10), so the apostrophe
 * that replaces it lands there. That one position ripples into SYMBOLS — see keymap.c.
 *
 * Symbol positions (22/32/33/34/35) are SY_* shifted-pair keycodes; the comment on
 * each shows its tap → shift output.
 */

// Row 0 (top row, columns 0-11)
#define _00_ KC_NO
#define _01_ KC_Q
#define _02_ KC_W
#define _03_ KC_E
#define _04_ KC_R
#define _05_ KC_T
#define _06_ KC_Y
#define _07_ KC_U
#define _08_ KC_I
#define _09_ KC_O
#define _10_ KC_P
#define _11_ KC_NO

// Row 1 (home row, columns 12-23)
// _12_ / _23_ are the Luz envelope (Tab / Backspace) and are placed by keymap.c,
// not here — they are frame, not layout.
#define _12_ KC_NO
#define _13_ KC_A
#define _14_ KC_S
#define _15_ KC_D
#define _16_ KC_F
#define _17_ KC_G
#define _18_ KC_H
#define _19_ KC_J
#define _20_ KC_K
#define _21_ KC_L
#define _22_ SY_QUOT             // ' → "   (replaces QWERTY's `;`)
#define _23_ KC_NO

// Row 2 (bottom row, columns 24-35)
#define _24_ KC_NO
#define _25_ KC_Z
#define _26_ KC_X
#define _27_ KC_C
#define _28_ KC_V
#define _29_ KC_B
#define _30_ KC_N
#define _31_ KC_M
#define _32_ SY_COMM             // , → ?
#define _33_ SY_DOT              // . → !
#define _34_ SY_SLSH             // / → |
#define _35_ SY_MINS             // - → _

// Thumb cluster (keys 36-41) — placed by keymap.c (Luz leaves thumbs free per variant)
#define _36_ KC_NO
#define _37_ KC_NO
#define _38_ KC_NO
#define _39_ KC_NO
#define _40_ KC_NO
#define _41_ KC_NO

// Base keycodes for the pos 32/33 mod-taps — plain comma/dot so the mod-tap tap
// bypasses the SY_COMM/SY_DOT custom keycodes (their ?/! shift is done in
// process_record_user instead).
#define _32_KC KC_COMM
#define _33_KC KC_DOT
