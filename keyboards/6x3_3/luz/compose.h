// Luz — Compose combo
// ----------------------------------------------------------------------------
// Canonical file at keyboards/6x3_3/luz/compose.h, shared by every Luz variant.
//
// Compose is armed by a single cross-hand combo on BASE — the only combo in Luz — and
// the whole thing is contractual: the POSITIONS, the OPERANDS and the behaviour are the
// same in every variant. Nothing here is per-layout, so nothing here belongs in keymap.c.
//
//     SHIFT (thumb 37) + SPACE (thumb 40)
//
// Why the thumbs and not two alphas: a combo operand must be a key you never type in
// sequence with the other operand. Shift is a MODIFIER, not a typed character, which is
// exactly what makes the pair safe — and since the mod contract puts a plain KC_LSFT on
// 37 and Space on 40 in every variant, the operands are identical everywhere. An alpha
// pair (the old positions 5+6) could not be: each layout puts different letters there, so
// the chord, the reference page and the README differed per variant for no gain.
//
// COMBO_MUST_PRESS_IN_ORDER (each variant's config.h) closes the one hazard: SHIFT must be
// pressed FIRST. Without it, rolling out of a Space into the next word's capital — right
// thumb still down, left thumb reaching for Shift — fires the combo within COMBO_TERM and
// swallows the space. Pressing Shift first is the natural way to make the gesture, so the
// constraint costs nothing and removes the only sequence that could trigger it by accident.
//
// Cost of putting the combo on these two keys: their presses are buffered for up to
// COMBO_TERM. In practice the buffer is dumped by the very next keystroke (a letter is not
// a combo key), and QMK replays the queued keyrecord with its ORIGINAL event.time — so
// held-Shift capitalisation keeps its ordering and the DOUBLE_TAP_SHIFT Caps Word detector
// (process_caps_word.c, which compares record->event.time) is unaffected.
//
// Wired in luz/luz.h:
//     combo_t key_combos[] = { COMBO_ACTION(compose_combo) };

#pragma once

#include "quantum.h"

// Order matters: COMBO_MUST_PRESS_IN_ORDER requires KC_LSFT (37) before KC_SPC (40).
const uint16_t PROGMEM compose_combo[] = {KC_LSFT, KC_SPC, COMBO_END};
