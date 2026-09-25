# Luz — open tap-hold tuning decisions

> This file is **not** part of the contract. [`LUZ.spec.md`](./LUZ.spec.md) states what a Luz
> variant must do; this states four settings that are *deliberately unresolved*, together with
> the arguments accumulated for and against each, so they don't get re-derived from scratch
> every time one of them comes up.
>
> None of the four can be settled by measurement. They are all *felt* — flash, type for a week,
> decide. What can be written down is the reasoning, and that is what this is.

Current state, in the shared `keyboards/6x3_3/luz/config.h`:

```c
#define TAPPING_TERM 240
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD
#define FLOW_TAP_TERM 150
```

`QUICK_TAP_TERM` is not defined, so it inherits `TAPPING_TERM`. `is_flow_tap_key()` is not
overridden, so QMK's default applies.

---

## 1. An `is_flow_tap_key()` override

**Status: the strongest candidate of the four. Additive, per-key, no structural cost.**

QMK's default implementation returns true only for `KC_A`–`KC_Z`, `KC_DOT`, `KC_COMM`,
`KC_SCLN`, `KC_SLSH` and `KC_SPC`, matched on the *tap* keycode. It is applied to **both** the
tap-hold key and the key pressed immediately before it — both must return true for Flow Tap to
engage. It also returns false outright whenever Ctrl, GUI or Alt is held ("disable Flow Tap on
hotkeys").

That default leaks in two opposite directions.

### Gap A — mod-taps with no protection at all

A mod-tap whose tap keycode is outside the default set gets **zero** Flow Tap coverage:

| Variant | Key | Tap | Why it matters |
|---|---|---|---|
| Enthium | BASE 18 | `RGUI_T(KC_MINS)` | one half of the morph pair |
| Enthium | BASE 31 | `RCTL_T(KC_QUOT)` | `'` is constant in French — *l'*, *d'*, *qu'*, *n'*, *s'* |

Gallium and Colemak-DH are fully covered: every one of their mod-tap taps is an alpha, a comma
or a dot.

### Gap B — a layer key that gets protection it may not want

Enthium's `LT(EXTEND, KC_R)` at position 38 taps an **alpha**, so Flow Tap *does* engage — which
means **EXTEND cannot be entered within `FLOW_TAP_TERM` (150 ms) of a keystroke.** Hold it too
soon after typing and you get `r` instead of the navigation layer.

`LT(SYMBOLS, KC_ENT)` at 39 is unaffected, because `KC_ENT` is not in the default set. That is
precisely why the SYMBOLS thumb has always felt instant and the EXTEND thumb, in Enthium only,
now does not.

### The decision inside the decision

Writing the override is trivial. The real question is what to do about Gap B, and it is a genuine
trade with no free side:

- **Flow Tap on** (today): `r` types reliably; entering EXTEND right after typing gives `r`.
- **Flow Tap off**: EXTEND is instant; a fast `r` rolled into the next letter can become a layer
  activation instead — and `r` is ~6% of letters.

Gap A has no such tension and should simply be closed.

**ZMK note:** `require-prior-idle-ms` is configured per behaviour, so neither gap exists there.
This is a QMK-shaped problem, not a Luz one.

---

## 2. `QUICK_TAP_TERM`

**Status: viable, but only as a per-key setting. A global `0` breaks something you use.**

Undefined, so it inherits `TAPPING_TERM` = 240 ms. The mechanism is the "sequential tap" branch
at `action_tapping.c:602`: press a dual-function key again within the window, having tapped it,
and QMK re-sends the **tap** keycode and holds it — so the OS auto-repeats — instead of engaging
the hold. The window is measured release-to-press, and it requires that nothing was pressed in
between.

### For `QUICK_TAP_TERM 0`

Type a letter, then immediately want that same key's modifier, and today you get auto-repeat of
the letter instead of the mod. `0` removes the auto-repeat path entirely and always gives you
the hold.

### Against

It **kills Enter auto-repeat.** Tap `LT(SYMBOLS, KC_ENT)` then press-and-hold within the window
and you currently get `⏎⏎⏎⏎`; with `0` you would get the SYMBOLS layer instead.

### Why the per-key form is clean here

Position 39 is the **only** tap-hold key in the framework where auto-repeat matters. Everything
else you would ever hold down to repeat is a plain keycode and is unaffected by any value of this
setting:

- Backspace — plain `KC_BSPC` (23 in Gallium/Colemak, 35 in Enthium)
- Delete, the arrows, PgUp/PgDn — all plain on EXTEND
- Space — plain at 40

The tap-hold keys that *would* lose auto-repeat are the morph pair's letters, the bottom-row mod
letters, and Enthium's `R` — none of which you hold to repeat in prose.

So: `QUICK_TAP_TERM_PER_KEY` with `get_quick_tap_term()` returning `0` for mod-taps and the
default for the SYMBOLS layer-tap.

**Depends on item 4:** `TAPPING_TERM` is both the default and the maximum legal value.

**ZMK note:** `quick-tap-ms`, set per behaviour.

---

## 3. `HOLD_ON_OTHER_KEY_PRESS` in place of `PERMISSIVE_HOLD`

**Status: considered and rejected. Recorded here so it is not re-proposed without new evidence.**

Chordal Hold is designed to pair with either, and it already settles every **same-hand** chord as
a tap. So this setting only governs the **opposite-hand** case, and the entire difference is:

| Sequence | Permissive Hold | HOLD_ON_OTHER_KEY_PRESS |
|---|---|---|
| **Roll** — press A, press B, release A, release B | **tap** | **hold** |
| **Nested** — press A, press B, release B, release A | hold | hold |

That is: *cross-hand rolls become modifier chords.* Cross-hand alternation is what a good alpha
layout maximises.

### What it would buy

- Sub-layer entry (`EXTEND_DEL`, `EXTEND_TABS`) stops waiting out the tapping term. Those
  triggers are `MO` keys — pressed and **held**, never released early — so Permissive Hold cannot
  settle the `LT` beneath them, and entry costs up to 240 ms.
- Layer entry generally becomes decisive.

### What it would cost

1. **Word-initial mod-tap letters misfire.** Flow Tap covers mid-flow typing, so the exposure is
   specifically *pause > 150 ms, then a mod-tap letter, then an opposite-hand letter* — which is
   exactly what starting to type after thinking looks like. Gallium's worst case is `W` at 27
   (GUI, left hand): *we, with, was, what, when* followed by a right-hand vowel → `⌘E`. `G` at 17
   does the same for *go, get, good*. Colemak-DH escapes it — its bottom-row `H` is right-hand,
   and H-words continue with right-hand vowels, so those chords stay same-hand and safe.
2. **Punctuation rolled into Space.** `get_chordal_hold_default` returns true whenever *either*
   key is `'*'`, and the thumbs are — so Enthium's `.` and `,` mod-taps at 32/33 are chord-eligible
   against Space. `. ` after a sentence-end pause could become `Alt+Space`.
3. **Flow Tap stops being a supplement and becomes the primary guard**, which promotes item 1
   from theoretical to load-bearing.

### Why the per-key escape does not rescue it

`HOLD_ON_OTHER_KEY_PRESS_PER_KEY` exists, so enabling it on only the layer keys looks tempting:

- **On 39:** `KC_ENT` is not a flow-tap key, so Flow Tap never protects it. Rolling Enter into the
  next character — plausible at speed — would give SYMBOLS + that letter. This works today.
- **On 38:** only Enthium has an `LT` there; Gallium and Colemak-DH have a bare `MO` with no
  tap/hold decision, so **the 240 ms problem is Enthium-only.** And enabling it on
  `LT(EXTEND, KC_R)` trades that delay for word-initial-`r` misfires — *rien*, *rue*, *rendre* →
  EXTEND + letter.

### The verdict

The problem it solves is **latency, not error**, it exists in **one variant**, and QMK buffers
the interrupting events — so everything still fires correctly, just late, once per sub-mode entry.
Trading that for a class of wrong actions is the wrong direction. If the lead-in becomes annoying,
item 4 is the proportionate lever.

**ZMK note:** `flavor = "balanced"` ≈ Permissive Hold; `flavor = "hold-preferred"` ≈ this.

---

## 4. `TAPPING_TERM` 240 → 180–200

**Status: a dial, not a policy. Low risk, try it whenever.**

240 ms is long; the usual home-row-mods range is 150–200.

### For

- Shortens the Enthium sub-layer lead-in from item 3 proportionally — the cheap answer to that
  problem.
- Deliberate holds land sooner when nothing else is pressed.

### Against

- It is also the window in which Chordal Hold and Permissive Hold apply at all — *"Chordal Hold
  has no effect after the tapping term."* Shortening it means the opposite-hands guard stops
  guarding sooner, and an opposite-hand chord becomes a hold by timeout earlier.
- `QUICK_TAP_TERM` inherits it, so lowering this also shrinks the auto-repeat window in item 2.

`TAPPING_TERM_PER_KEY` exists if a single position turns out to want a different value.

**ZMK note:** `tapping-term-ms`.

---

## How the four interact

- **2 inherits from 4.** `QUICK_TAP_TERM` defaults to `TAPPING_TERM` and may not exceed it.
- **3 would make 1 mandatory.** Adopting HOLD_ON_OTHER_KEY_PRESS turns Flow Tap into the only
  remaining guard, so the gaps in item 1 would stop being theoretical.
- **4 partially substitutes for 3.** The Enthium lead-in is the only thing 3 would fix, and 4
  shortens it at a fraction of the risk.

## Summary

| # | Setting | Weight | Recommendation |
|---|---------|--------|----------------|
| 1 | `is_flow_tap_key()` override | high | close Gap A outright; Gap B is a real trade to make deliberately |
| 2 | `QUICK_TAP_TERM` | medium | only per-key — `0` for mod-taps, default for position 39 |
| 3 | `HOLD_ON_OTHER_KEY_PRESS` | negative | do not adopt |
| 4 | `TAPPING_TERM` 240 → 180–200 | low | safe to try; the proportionate answer to what 3 would have fixed |
