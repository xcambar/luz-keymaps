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
//   - Home-row INDEX morph (16 left, 19 right): a GUI mod-tap that becomes Ctrl on
//     Linux (see LUZ_INDEX_GUI_MORPH) — the unified Cmd/Ctrl shortcut key.
//   - Chordal Hold (opposite-hands rule) arbitrates every mod-tap; thumbs are exempt.
//
// Companion config (kept in each variant's config.h, identical, part of the contract):
//   TAPPING_TERM 240, CHORDAL_HOLD, PERMISSIVE_HOLD, FLOW_TAP_TERM 150.

#pragma once

#include <stdint.h>
#include "quantum.h"
#include "features/os_control.h"

// Chordal Hold handedness: 'L'=left, 'R'=right, '*'=exempt (thumbs). Purely
// positional — identical for every variant, so it lives here, not in keymap.c.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
    LAYOUT_split_3x6_3(
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
        'L', 'L', 'L', 'L', 'L', 'L',  'R', 'R', 'R', 'R', 'R', 'R',
                       '*', '*', '*',  '*', '*', '*'
    );

// Home-row index Cmd/Ctrl morph. The index home keys (pos 16/19) are GUI mod-taps;
// on macOS GUI is correct as-is, on Linux the HOLD should be Ctrl instead. This
// expands inside process_record_user (like SYM_MODTAP_SHIFT): when the morph key is
// held (not tapped) on a non-macOS platform, register Ctrl and return false.
//   morph_l / morph_r are the snapshotted LGUI_T()/RGUI_T() keycodes for pos 16/19,
//   defined in keymap.c since they wrap a per-layout letter.
// Needs `keycode` and `record` in scope; get_os_platform() from features/os_control.h.
#define LUZ_INDEX_GUI_MORPH(keycode, record, morph_l, morph_r) \
    do { \
        if (get_os_platform() != OS_MacOS && !(record)->tap.count) { \
            if ((keycode) == (morph_l)) { \
                if ((record)->event.pressed) register_code(KC_LCTL); else unregister_code(KC_LCTL); \
                return false; \
            } \
            if ((keycode) == (morph_r)) { \
                if ((record)->event.pressed) register_code(KC_RCTL); else unregister_code(KC_RCTL); \
                return false; \
            } \
        } \
    } while (0)
