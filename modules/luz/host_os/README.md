# luz/host_os

Which operating system the keyboard is talking to, as a **setting** other modules read to
pick OS-specific chords (`luz/semantic_keys`, `luz/dead_keys`, `luz/cmd_ctrl_morph`). It is
not detection: you set it with a key, and it starts at a configured default.

| Keycode | Alias | Does |
|---|---|---|
| `COMMUNITY_MODULE_HOST_OS_TOGGLE` | `CM_OSTG` | switch to the next OS (macOS ↔ Linux) |
| `COMMUNITY_MODULE_HOST_OS_PRINT` | `CM_OSPR` | type the current OS's name |

| `config.h` | Default |
|---|---|
| `HOST_OS_DEFAULT` | `HOST_OS_LINUX` |

API (`host_os.h`): `host_os_get()`, `host_os_set(os)`, `host_os_toggle()`,
`host_os_name()`, with `host_os_t` being `HOST_OS_MACOS` or `HOST_OS_LINUX`.

The setting is not persisted: it returns to the default when the keyboard restarts.
