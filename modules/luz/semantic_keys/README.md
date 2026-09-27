# luz/semantic_keys

One keycode per editing **intent**, sending whatever chord means it on the host OS (read
from `luz/host_os`). Put `CM_COPY` on a key and it is ⌘C on macOS and Ctrl+C on Linux.

| Keycode (`COMMUNITY_MODULE_SEMANTIC_…`) | Alias | macOS | Linux |
|---|---|---|---|
| `UNDO` | `CM_UNDO` | ⌘Z | Ctrl+Z |
| `CUT` / `COPY` / `PASTE` | `CM_CUT` / `CM_COPY` / `CM_PSTE` | ⌘X / ⌘C / ⌘V | Ctrl+X / C / V |
| `EURO` | `CM_EURO` | ⌥⇧2 | AltGr+=, E |
| `CEDILLA` | `CM_CEDL` | ⌥C | AltGr+,, C |
| `WORD_PREV` / `WORD_NEXT` | `CM_WPRV` / `CM_WNXT` | ⌥← / ⌥→ | Ctrl+← / → |
| `LINE_BEGIN` / `LINE_END` | `CM_LBEG` / `CM_LEND` | ⌘← / ⌘→ | Home / End |
| `DEL_WORD_PREV` / `DEL_WORD_NEXT` | `CM_DWBK` / `CM_DWFW` | ⌥⌫ / ⌥⌦ | Ctrl+⌫ / Ctrl+Del |
| `DEL_LINE_BEGIN` / `DEL_LINE_END` | `CM_DLBK` / `CM_DLFW` | ⌘⌫ / Ctrl+K | ⇧Home ⌫ / ⇧End ⌫ |
| `TAB_NEW` / `TAB_CLOSE` / `TAB_REOPEN` | `CM_TNEW` / `CM_TCLS` / `CM_TROP` | ⌘T / ⌘W / ⌘⇧T | Ctrl+T / W / ⇧T |
| `TAB_LEFT` / `TAB_RIGHT` | `CM_TPRV` / `CM_TNXT` | ⌘⌥← / → | Ctrl+PgUp / PgDn |
| `HISTORY_PREV` / `HISTORY_NEXT` | `CM_HBCK` / `CM_HFWD` | ⌘[ / ⌘] | Alt+← / → |

- **Single-chord keys are held while the key is held**, so they auto-repeat like the real
  chord. Multi-step ones (the Linux line deletions, €, ç) are tapped.
- **Shift + `CM_COPY` cuts**, consuming the Shift.
- The Linux characters assume a layout with AltGr compose sequences (EurKEY, US-intl).
- The motion chords follow [HandsDown](https://github.com/moutis/HandsDown)'s semantic
  keys.

API (`semantic_keys.h`): `tap_semkey_code(keycode)` taps a semantic key's chord from your
own code.

Requires `luz/host_os`.
