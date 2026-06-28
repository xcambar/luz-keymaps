// Luz — shared layer model
// ----------------------------------------------------------------------------
// This header is the SINGLE SOURCE OF TRUTH for the Luz layer set, shared by every
// Luz variant (luz_for_gallium = Luz for Gallium, luz_for_enthium = Luz for Enthium).
// Each variant's rules.mk puts this dir's parent on the include path
// (`VPATH += $(QMK_USERSPACE)/keyboards/6x3_3`), so keymap.c can `#include "luz/layers.h"`.
//
// See LUZ.md (repo root) for the prose spec. The rules in brief:
//
//   - BASE (layer 0) is the ONLY layout-specific layer; it carries the
//     underlying alpha layout (Gallium East / Enthium). Every layer above BASE
//     is identical across all Luz variants — same output at the same position
//     on every layer (cross-layer consistency).
//
//   - EXTEND is the navigation + shortcut hub, held via the left inner thumb.
//     It is the launch point for the hold-only sub-layers, whose names carry
//     the EXTEND_ prefix to advertise that parentage:
//       * EXTEND_DEL  — destructive deletion sub-layer
//       * EXTEND_TABS — browser tab management sub-layer
//
//   - SYMBOLS is the numpad (left) + symbol field (right), held via the right
//     inner thumb (layer-tap with Enter).
//
//   - ADJUST is the tri-layer (EXTEND + SYMBOLS held together): F-keys, media,
//     and the only bootloader path.
//
// Layer ORDER is contractual: sub-layers and ADJUST must sit above the two base
// overlays so their transparency chains resolve correctly.

#pragma once

enum luz_layers {
    BASE = 0,    // layout-specific alpha layer (Gallium / Enthium)
    EXTEND,      // nav + shortcuts hub (left inner thumb hold)
    SYMBOLS,     // numpad + symbol field (right inner thumb)
    EXTEND_DEL,  // deletion sub-layer (hold-only, launched from EXTEND)
    EXTEND_TABS, // browser tab management sub-layer (hold-only, from EXTEND)
    ADJUST       // F-keys / media / boot (tri-layer: EXTEND + SYMBOLS)
};
