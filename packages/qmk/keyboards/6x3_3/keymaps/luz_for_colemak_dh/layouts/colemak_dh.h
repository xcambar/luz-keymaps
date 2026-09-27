// Colemak Mod-DH (matrix / "DHm") Layout Key Definitions
// This file maps abstract key positions (_XX_) to actual Colemak-DH keycodes.
// The matrix variant is the one intended for ortho and column-staggered boards
// (D and H on the bottom-row index columns), and is the same arrangement Miryoku
// and Seniply ship on.
// https://colemakmods.github.io/mod-dh/

/*
 * Colemak Mod-DH (matrix) - Actual Keycodes
 *
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │  Q  │  W  │  F  │  P  │  B  │       │  J  │  L  │  U  │  Y  │  '  │     │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │     │  A  │  R  │  S  │  T  │  G  │       │  M  │  N  │  E  │  I  │  O  │     │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │     │  Z  │  X  │  C  │  D  │  V  │       │  K  │  H  │  ,  │  .  │  /  │  -  │
 * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
 *                     ┌─────┐                           ┌─────┐
 *                     │     ├─────┐               ┌─────┤     │
 *                     └─────┤     ├─────┐   ┌─────┤     ├─────┘
 *                           └─────┤     │   │     ├─────┘
 *                                 └─────┘   └─────┘
 *
 * Apostrophe replaces Semicolon at pos 10, the usual 30-key Colemak convention
 * (Miryoku does the same); `;` stays reachable as SY_COLN's shifted glyph on SYMBOLS.
 *
 * Privileged BASE symbols (Luz principle 1) are ' , . / - — the same five Gallium East
 * privileges, but `/` lands at 34 because it belongs to the Colemak-DH alpha block, so
 * `-` takes the outer column at 35 (Gallium has these two the other way round).
 *
 * Symbol positions (10/32/33/34/35) are SY_* shifted-pair keycodes; the comment on
 * each shows its tap → shift output.
 */

// Row 0 (top row, columns 0-11)
#define _00_ KC_NO
#define _01_ KC_Q
#define _02_ KC_W
#define _03_ KC_F
#define _04_ KC_P
#define _05_ KC_B
#define _06_ KC_J
#define _07_ KC_L
#define _08_ KC_U
#define _09_ KC_Y
#define _10_ SY_QUOT             // ' → "
#define _11_ KC_NO

// Row 1 (home row, columns 12-23)
// _12_ / _23_ are the Luz envelope (Tab / Backspace) and are placed by keymap.c,
// not here — they are frame, not layout.
#define _12_ KC_NO
#define _13_ KC_A
#define _14_ KC_R
#define _15_ KC_S
#define _16_ KC_T
#define _17_ KC_G
#define _18_ KC_M
#define _19_ KC_N
#define _20_ KC_E
#define _21_ KC_I
#define _22_ KC_O
#define _23_ KC_NO

// Row 2 (bottom row, columns 24-35)
#define _24_ KC_NO
#define _25_ KC_Z
#define _26_ KC_X
#define _27_ KC_C
#define _28_ KC_D
#define _29_ KC_V
#define _30_ KC_K
#define _31_ KC_H
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
