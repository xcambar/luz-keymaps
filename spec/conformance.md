# 7. Conformance

An implementation is a **Luz variant** when it meets every MUST in chapters 1–6. This
chapter is the short version to check against, and the way to check it automatically.

## Checklist

**Frame and layers**
- [ ] 37 plain Shift, 40 Space, 38 EXTEND (hold), 39 SYMBOLS (hold) / Enter (tap), 36 and
      41 blank.
- [ ] Tab and Backspace on the outer columns, together on the home or the bottom row.
- [ ] No dual-function keys on the home row (13–16, 19–22).
- [ ] Six layers, reached as specified. The sub-layers are hold-only, ADJUST is the
      tri-layer, and the bootloader is only on ADJUST.
- [ ] Same output, same position, on every layer where it appears.

**Modifiers**
- [ ] Mod-taps: 26/27/28 = Alt/GUI/Ctrl, 31/32/33 = Ctrl/GUI/Alt(Gr).
- [ ] Cmd/Ctrl morph on 17/18: GUI on macOS, Ctrl otherwise, left and right kept.
- [ ] Tap-hold: 240 ms tapping term, opposite-hands rule (thumbs exempt), permissive
      hold, 150 ms fast-typing protection.
- [ ] Caps Word on double-tap Shift, `-` becoming `_`.
- [ ] Mod latch on SYMBOLS: no Shift, dropped when SYMBOLS is locked.

**Symbols, Compose, navigation**
- [ ] The sixteen symbols and their Shift pairs, on any layer and on mod-taps.
- [ ] The SYMBOLS left half exactly as specified.
- [ ] Compose on Shift-then-Space; E A U O N dead keys, C ç, W €, Esc cancels, modifiers
      don't consume it, anything else passes through.
- [ ] EXTEND, EXTEND_DEL, EXTEND_TABS and ADJUST as specified. Fills MAY differ, geometry
      and sub-mode rules MUST NOT.

Within this, a variant chooses its letters, its privileged BASE symbols, its SYMBOLS right
half, the taps under the morph and the EXTEND thumb, and where its envelope sits
([What a variant may change](README.md#what-a-variant-may-change)).

## Executable conformance

The rules are also written as **scenarios**: key presses by position, and the USB reports
the host must receive, with exact timings where timing is the rule. They live in
[`tests/scenarios/`](../tests/scenarios/):

| File | Checks |
|---|---|
| [`luz.yaml`](../tests/scenarios/luz.yaml) | what every variant shares: the holds, EXTEND and its sub-layers, the SYMBOLS left half and its macros, the mod latch, ADJUST, the host OS setting |
| `luz_for_<variant>.yaml` | what depends on that variant's letters: typing, symbol pairs on BASE, tap-hold rolls, Caps Word, Compose |

Scenarios don't know about firmware, keycodes or matrices. They run with
[keyspec](../packages/keyspec/): a **runner adapter** replays them on an implementation and
reports what the host would have received. The QMK adapter runs the reference
implementations in QMK's test harness; CI runs all four
variants on both boards, on every pull request and on every push to `main`.

**A new variant, on any firmware,** should pass `luz.yaml` unchanged, plus a file of its own
written the same way. For firmware other than QMK, that takes a keyspec adapter for it (ZMK's
native POSIX build is the natural candidate).

*Note.* Scenarios are exact about the report stream, which is sometimes more than this
specification requires. For example, a mod-tap symbol's Shift partner is typed by briefly
releasing Shift. What the spec requires is the host's result (`?`, not `,`), so another
implementation may produce a different stream and still conform. Where scenarios and
chapters 1–6 disagree, the chapters win and the scenario is adjusted.
