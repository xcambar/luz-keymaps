// Luz — shared mod system
// ----------------------------------------------------------------------------
// Canonical file at keyboards/6x3_3/luz/mods.h, shared by every Luz variant. Each
// variant's rules.mk puts this dir's parent on the include path
// (`VPATH += $(QMK_USERSPACE)/keyboards/6x3_3`), so keymap.c can `#include "luz/mods.h"`.
// See LUZ.md (repo root) for the prose spec.
//
// The MOD PLACEMENT and BEHAVIOR are a Luz contract: every variant carries the same
// modifier on the same physical position, so chording for shortcuts is identical
// across layouts. Only the letters UNDER the mods change per layout (that is just
// BASE). What lives here is the part that is positional and therefore shareable.
//
// The scheme (positions per CLAUDE.md's split_3x6_3 map):
//   - Bottom-row mod-taps, mirrored Alt-GUI-Ctrl outward->inward:
//       left  26=Alt  27=GUI  28=Ctrl      right 31=Ctrl 32=GUI 33=Alt
//   - SHIFT is a plain modifier on thumb 37 — not a mod-tap at all. Thumbs are '*'
//     (exempt) in chordal_hold_layout, so one key holds for BOTH hands; the mod you hold
//     most therefore costs no tap-hold arbitration and needs no mirrored twin.
//   - Cmd/Ctrl MORPH mirrored on the INNER INDEX column (17 left, 18 right) — GUI on
//     macOS, Ctrl on Linux, so Cmd-C and Ctrl-C are the same chord. Mirrored because it IS
//     subject to the opposite-hands rule. 17/18 is the lightest pair in every layout
//     (~0.8-4.4% of English letters); the home row is not, and in Gallium (T/H) and
//     Enthium (H/E) positions 16/19 span a common CROSS-HAND bigram — exactly the case
//     Chordal Hold does not cover — so the morph must not live there.
//   - The home row (15/16/19/20) carries NO mod-taps.
//   - What sits UNDER a pinned hold is free per variant: the morph's taps are letters, and
//     a variant may attach a tap to the EXTEND hold on 38 (Enthium: LT(EXTEND, KC_R)).
//   - Chordal Hold (opposite-hands rule) arbitrates every mod-tap; thumbs are exempt.
//   - LAYER-SCOPED MOD LATCH: while SYMBOLS is up, releasing a held modifier latches it
//     for the life of the layer instead of releasing it, so Ctrl/Alt/Cmd + a SYMBOLS key
//     needs only one hand. Shift is excluded and Layer Lock drops the latch. Implemented
//     by the luz/mod_latch community module (MOD_LATCH_LAYER in luz/config.h).
//
// Companion config (kept in each variant's config.h, identical, part of the contract):
//   TAPPING_TERM 240, CHORDAL_HOLD, PERMISSIVE_HOLD, FLOW_TAP_TERM 150,
//   DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD (thumb 37 is a plain KC_LSFT, which the
//   built-in detector requires; BOTH_SHIFTS cannot work with a single Shift key).
//
// KNOWN GAPS around Flow Tap (an is_flow_tap_key() / get_flow_tap_term() override is the
// fix for both):
//   1. The default is_flow_tap_key() covers KC_A-Z, KC_DOT, KC_COMM, KC_SCLN, KC_SLSH and
//      KC_SPC only, so mod-taps tapping anything else get no protection: Enthium's
//      RGUI_T(KC_MINS) at 18 and RCTL_T(KC_QUOT) at 31. (SYMBOLS carries no mod-taps at all
//      since the morph was removed from it, so BASE is the whole exposure.)
//   2. Conversely, Enthium's LT(EXTEND, KC_R) at 38 taps an ALPHA, so Flow Tap *does*
//      engage — which means EXTEND cannot be entered within FLOW_TAP_TERM of a keystroke.
//      LT(SYMBOLS, KC_ENT) is unaffected because KC_ENT is not a flow-tap key.

#pragma once

#include <stdint.h>
#include "quantum.h"

// Chordal Hold handedness: 'L'=left, 'R'=right, '*'=exempt (thumbs). Purely
// positional — identical for every variant, so it lives here, not in keymap.c.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_split_3x6_3(
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
                       '*', '*', '*',  '*', '*', '*'
    );

// Cmd/Ctrl morph — the unified shortcut modifier: GUI on macOS, Ctrl on Linux, so
// Cmd-C and Ctrl-C are the same chord. It is a mirrored pair of GUI mod-taps on the inner
// index column (17/18), implemented by the luz/cmd_ctrl_morph community module; luz/luz.h
// tells it which keys morph (the variant's LUZ_MORPH_L/LUZ_MORPH_R). The morph exists on
// BASE only: modifiers live on BASE and are held BEFORE switching layers, which is already
// the required pattern for Alt (26/33) since the bottom row is occupied by digits and
// symbols on SYMBOLS.
