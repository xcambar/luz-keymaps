// Gallium v2 Layout Key Definitions
// This file maps abstract key positions (_XX_) to actual Gallium v2 keycodes
// Gallium v2 (Rowstag) is optimized for row-staggered keyboards
// https://github.com/GalileoBlues/Gallium

/*
 * Gallium v2 EAST - Actual Keycodes
 * https://www.reddit.com/r/KeyboardLayouts/comments/1rym73o/introducing_galliumeast/
 * https://cyanophage.github.io/playground.html?layout=blcdvjfou%2C-nrstgyheai%2Fqxwmzkp%27%3B.%5C%5E&mode=ergo&lan=english&thumb=l
 *
 * ┌─────┬─────┬─────┬─────┬─────┬─────┐       ┌─────┬─────┬─────┬─────┬─────┬─────┐
 * │     │  B  │  L  │  C  │  D  │  V  │       │  J  │  F  │  O  │  U  │  '  │     │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │     │  N  │  R  │  S  │  T  │  G  │       │  Y  │  H  │  E  │  A  │  I  │  /  │
 * ├─────┼─────┼─────┼─────┼─────┼─────┤       ├─────┼─────┼─────┼─────┼─────┼─────┤
 * │     │  X  │  Q  │  W  │  M  │  Z  │       │  K  │  P  │  ,  │  .  │  -  │     │
 * └─────┴─────┴─────┴─────┴─────┴─────┘       └─────┴─────┴─────┴─────┴─────┴─────┘
 *                     ┌─────┐                           ┌─────┐
 *                     │     ├─────┐               ┌─────┤     │
 *                     └─────┤     ├─────┐   ┌─────┤     ├─────┘
 *                           └─────┤     │   │     ├─────┘
 *                                 └─────┘   └─────┘
 *
 * Symbol positions (10/23/32/33/34) are SY_* shifted-pair keycodes; the comment
 * on each shows its tap → shift output.
 */

// Row 0 (top row, columns 0-11)
#define _00_ KC_NO
#define _01_ KC_B
#define _02_ KC_L
#define _03_ KC_C
#define _04_ KC_D
#define _05_ KC_V
#define _06_ KC_J
#define _07_ KC_F
#define _08_ KC_O
#define _09_ KC_U
#define _10_ SY_QUOT             // ' → "
#define _11_ KC_NO

// Row 1 (home row, columns 12-23)
#define _12_ KC_NO
#define _13_ KC_N
#define _14_ KC_R
#define _15_ KC_S
#define _16_ KC_T
#define _17_ KC_G
#define _18_ KC_Y
#define _19_ KC_H
#define _20_ KC_E
#define _21_ KC_A
#define _22_ KC_I
#define _23_ SY_SLSH             // / → | (dead on BASE — KC_BSPC overrides pos 23)

// Row 2 (bottom row, columns 24-35)
#define _24_ KC_NO
#define _25_ KC_X
#define _26_ KC_Q
#define _27_ KC_W
#define _28_ KC_M
#define _29_ KC_Z
#define _30_ KC_K
#define _31_ KC_P
#define _32_ SY_COMM             // , → ?
#define _33_ SY_DOT              // . → !
#define _34_ SY_MINS             // - → _
#define _35_ KC_NO

// Thumb cluster (keys 36-41)
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
