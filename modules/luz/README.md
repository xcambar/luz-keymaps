# Luz community modules for QMK

The features behind [Luz](../../README.md), packaged as
[QMK Community Modules](https://docs.qmk.fm/features/community_modules) so any keymap can
use them, Luz or not. Each one is small, does one thing, and is configured from your
`config.h` or a callback in your keymap.

| Module | What it does | Needs |
|---|---|---|
| [`luz/compose`](compose/) | Luz's Compose: arm it, and the next key picks an accent or character | `luz/dead_keys`, Luz's semantic keys (for now) |
| [`luz/dead_keys`](dead_keys/) | Keys that tap the host's dead key for an accent (´ ` ^ ¨ ~) | Luz's OS setting (for now) |

## Using them

Put this directory at `modules/luz/` in your
[external userspace](https://docs.qmk.fm/newbs_external_userspace), then list the modules
in your keymap's `keymap.json`:

```json
{
    "modules": [
        "luz/compose",
        "luz/dead_keys"
    ]
}
```

**Order matters:** QMK hands every key press to the modules in the listed order, before
your `process_record_user`, and a module that consumes a key hides it from the ones after
it.

## Tests

The behaviour of every module is pinned by the Luz scenarios in
[`tests/scenarios/`](../../tests/scenarios/), run with [keyspec](../../packages/keyspec/)
in QMK's test harness, modules included.
