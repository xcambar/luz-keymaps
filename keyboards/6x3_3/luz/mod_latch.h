// Luz — layer-scoped mod latch
// ----------------------------------------------------------------------------
// Canonical file at keyboards/6x3_3/luz/mod_latch.h, shared by every Luz variant.
// Pulled in by luz/mods.h, because it is part of the mod contract, not a variant
// feature. See LUZ.spec.md (repo root) for the prose spec.
//
// THE PROBLEM
// -----------
// Modifiers live on BASE only: the bottom-row mod-taps (26/27/28 | 31/32/33) and the
// morph pair (17/18) are letters and symbols on SYMBOLS, so a chord that needs both a
// modifier and a SYMBOLS key must be built by holding the modifier FIRST and entering
// the layer under it. That works, but the modifier then has to stay physically held for the
// whole chord, pinning a finger — and on SYMBOLS the mods are on the bottom row while the
// digits sit on the same hand's ring/middle/index, so holding left Ctrl at 28 while pressing
// 5 at 27 asks two adjacent fingers of one hand to do different jobs. These are also the
// chords you repeat rather than perform once: Ctrl/Alt/Cmd + a digit, keying figures into a
// spreadsheet.
//
// THE RULE
// --------
// While SYMBOLS is active, releasing a held modifier does NOT release it. The modifier
// is LATCHED for as long as the layer lasts, and dropped when the layer goes away:
//
//     hold Alt  ->  Alt registered
//     hold SYMBOLS
//     release Alt  ->  Alt STAYS registered   (latched)
//     ... type on SYMBOLS with no finger pinned on Alt ...
//     release SYMBOLS  ->  Alt released
//
// The gesture is timing-independent: it is not a tap, a double-tap or a one-shot, so
// there is no term to learn and no window to miss. It is the ordinary hold you already
// perform, read differently because a layer is up.
//
// THREE DELIBERATE LIMITS
// -----------------------
//   1. SYMBOLS ONLY. EXTEND is a layer of whole commands (its semantic keys build their
//      own chords), so a latched modifier there would corrupt them. ADJUST inherits the
//      latch because it IS the SYMBOLS thumb plus the EXTEND thumb — releasing the
//      SYMBOLS thumb ends both the layer and the latch, which is the same rule, not an
//      exception to it.
//   2. SHIFT NEVER LATCHES. Shift is the plain thumb key at 37 and the one modifier
//      used inside ordinary typing on this layer — every SY_* symbol's shifted partner
//      is reached with it. A latched Shift would turn the whole symbol field over to its
//      shifted face until the layer ended, which is not a mode anyone asked for. Shift
//      is also not a mod-tap, so it never reaches this code; the mask below states the
//      rule anyway, so a future shifted mod-tap cannot acquire the behaviour by accident.
//   3. LAYER LOCK DROPS THE LATCH. QK_LLCK makes SYMBOLS persist without a thumb, i.e.
//      turns a chord into a mode. A modifier held across that boundary would outlive
//      every cue that it is down. Locking is the explicit "I am staying here" gesture,
//      so it is the right place to let the modifiers go.
//
// WHY IT IS SAFE
// --------------
// No Luz layer above BASE places a mod-tap anywhere. A mod-tap RELEASE seen while
// SYMBOLS is up therefore always belongs to a key pressed on BASE — QMK resolves a
// release against the layer cached at press time (quantum.c get_event_keycode /
// action.c store_or_get_action), so the BASE keycode is what arrives here. The latch
// can never fire on a key that was pressed under the layer, because no such key exists.
//
// ZMK NOTE
// --------
// There is no direct equivalent. The closest construction is a `&sl`-style sticky
// behaviour scoped by a conditional layer, which is a different gesture (sticky is
// consumed by the next key; this persists for the layer). An implementation on other
// firmware should reproduce the RULE above, not this mechanism.

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "quantum.h"
#include "layer_lock.h"
#include "luz/layers.h"

// Modifiers currently latched, as a MOD_BIT mask. Non-zero only while SYMBOLS is up.
static uint8_t luz_latched_mods = 0;

// Release everything the latch is holding. Idempotent.
static inline void luz_mod_latch_drop(void) {
    if (luz_latched_mods) {
        unregister_mods(luz_latched_mods);
        luz_latched_mods = 0;
    }
}

// Offer `mods` (a MOD_BIT mask, already registered) to the latch. Returns true if the
// latch took ownership — the caller must then NOT unregister them.
static inline bool luz_mod_latch_take(uint8_t mods) {
    mods &= ~MOD_MASK_SHIFT;                                  // limit 2
    if (!mods) return false;
    if (!layer_state_cmp(layer_state, SYMBOLS)) return false; // limit 1
    if (is_layer_locked(SYMBOLS)) return false;               // limit 3
    luz_latched_mods |= mods;
    return true;
}

// Mod-tap entry point, called from process_record_user. The luz/cmd_ctrl_morph module runs
// before it: off macOS the morph registers its Ctrl by hand and offers it to the latch itself
// (mod_latch_take, in luz/luz.h).
// Returns false when the release was swallowed, i.e. the modifier is now latched.
static inline bool luz_mod_latch_process(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed || record->tap.count != 0 || !IS_QK_MOD_TAP(keycode)) {
        return true;  // press, tap, or not a mod-tap at all
    }
    // Same derivation QMK uses to register them (keymap_common.c / action.c): the 5-bit
    // field's 0x10 bit selects the right-hand half of the MOD_BIT mask. mod_config()
    // keeps bootmagic mod swaps in sync, so we unregister exactly what was registered.
    const uint8_t mod  = mod_config(QK_MOD_TAP_GET_MODS(keycode));
    const uint8_t mods = (mod & 0x10) ? ((mod & 0x0F) << 4) : (mod & 0x0F);
    return !luz_mod_latch_take(mods);
}

// Layer entry point. Call from layer_state_set_user with the POST-tri-layer state.
static inline void luz_mod_latch_layer_state(layer_state_t state) {
    if (!layer_state_cmp(state, SYMBOLS)) {
        luz_mod_latch_drop();
    }
}

// Layer Lock entry point — limit 3. Defined here rather than left to each keymap.c
// because it is contractual and identical everywhere (same reason chordal_hold_layout
// lives in luz/mods.h). A variant that needs its own lock callback should move this
// body into it.
bool layer_lock_set_user(layer_state_t locked_layers) {
    if (locked_layers & ((layer_state_t)1 << SYMBOLS)) {
        luz_mod_latch_drop();
    }
    return true;
}
