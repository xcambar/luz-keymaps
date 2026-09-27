# Luz community modules for QMK

The features behind [Luz](../../README.md), packaged as
[QMK Community Modules](https://docs.qmk.fm/features/community_modules) so any keymap can
use them, Luz or not. Each one is small, does one thing, and is configured from your
`config.h` or a callback in your keymap.

| Module | What it does | Needs |
|---|---|---|
| [`luz/host_os`](host_os/) | A host-OS setting (macOS / Linux) the other modules read; keys to toggle and print it | – |
| [`luz/semantic_keys`](semantic_keys/) | One key per editing intent (undo, copy, word left, delete line, new tab...), sending the right chord for the host OS | `host_os` |
| [`luz/dead_keys`](dead_keys/) | Keys that tap the host's dead key for an accent (´ ` ^ ¨ ~) | `host_os` |
| [`luz/compose`](compose/) | Luz's Compose: arm it, and the next key picks an accent or character | `dead_keys`, `semantic_keys` |
| [`luz/cmd_ctrl_morph`](cmd_ctrl_morph/) | GUI mod-taps that hold Ctrl off macOS, so ⌘C and Ctrl+C are one chord | `host_os` |
| [`luz/mod_latch`](mod_latch/) | While a chosen layer is up, a released modifier stays held until the layer ends | – |
| [`luz/swapper`](swapper/) | Cmd-Tab on one key: the modifier stays held while you tap through windows | – |

## Using them

Put this directory at `modules/luz/` in your
[external userspace](https://docs.qmk.fm/newbs_external_userspace), then list the modules
in your keymap's `keymap.json`:

```json
{
    "modules": [
        "luz/compose",
        "luz/dead_keys",
        "luz/semantic_keys",
        "luz/swapper",
        "luz/cmd_ctrl_morph",
        "luz/mod_latch",
        "luz/host_os"
    ]
}
```

**Order matters:** QMK hands every key press to the modules in the listed order, before
your `process_record_user`, and a module that consumes a key hides it from the ones after
it. The order above is the one Luz uses:
- `compose` comes first, so it sees the key after it is armed;
- `cmd_ctrl_morph` comes before `mod_latch`, so a morphed Ctrl can be latched.

Take only the modules you want, keeping their relative order.

Keycodes come with short `CM_*` aliases (`CM_COPY`, `CM_ACUT`, `CM_SWIN`...), or use the
full `COMMUNITY_MODULE_*` names. Each module's README lists its keycodes, settings and
API.

## Tests

The behaviour of every module is pinned by the Luz scenarios in
[`tests/scenarios/`](../../tests/scenarios/), run with [keyspec](../../packages/keyspec/)
in QMK's test harness, modules included.
