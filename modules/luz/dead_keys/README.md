# luz/dead_keys

Keys that tap the **host's own dead key** for an accent, so the next letter the host
receives carries it (`CM_ACUT`, then `e`, gives `é`). The chord depends on the host OS,
read from `luz/host_os`.

| Keycode (`COMMUNITY_MODULE_DEAD_…`) | Alias | Accent | macOS | Linux |
|---|---|---|---|---|
| `ACUTE` | `CM_ACUT` | ´ | ⌥E | AltGr+' |
| `GRAVE` | `CM_GRAV` | ` | ⌥` | AltGr+` |
| `CIRC` | `CM_CIRC` | ^ | ⌥I | AltGr+^ |
| `DIAE` | `CM_DIAE` | ¨ | ⌥U | AltGr+" |
| `TILDE` | `CM_TILD` | ~ | ⌥N | AltGr+~ |

The macOS chords are the standard US layout's. The Linux ones assume a layout whose AltGr
layer carries dead keys (EurKEY, US-intl with AltGr dead keys).

API (`dead_keys.h`): `tap_deadkey_code(keycode)` taps a dead key from your own code
(`luz/compose` uses it).

Requires `luz/host_os`.
