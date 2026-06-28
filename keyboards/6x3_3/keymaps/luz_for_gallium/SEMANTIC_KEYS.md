# Semantic Keys Implementation

This implementation provides platform-independent editing commands that automatically send the correct keystroke for Mac or Linux.

## Architecture

### Data Structure
Semantic keys use a lookup table (`semkey_table`) that holds platform-specific keycodes:

```c
typedef struct {
    uint16_t mac_keycode;
    uint16_t linux_keycode;
} semkey_map_t;
```

### How It Works

1. **Key Press**: When you press a semantic key (e.g., `SK_COPY`)
2. **Translation**: `process_semkey()` looks up the platform-specific keycode
3. **Execution**: The platform keycode is sent (e.g., `Cmd+C` on Mac or `Ctrl+C` on Linux)
4. **Transparent**: QMK's core handles all modifiers (oneshot, key_override, etc.)

## Compatibility

### ✅ Callum-Style Oneshot Modifiers
The semantic keys work seamlessly with Callum-style oneshots because:
- Semantic keys simply translate to platform-specific keycodes
- They don't interfere with modifier state
- Oneshot processing happens in `process_record_user()` after semantic key translation

### ✅ QMK Key Override
The implementation is fully compatible with `key_override`:

```c
// Shift + SK_COPY → SK_CUT
const key_override_t copy_to_cut_override = ko_make_basic(MOD_MASK_SHIFT, SK_COPY, SK_CUT);
```

**How it works together:**
1. User presses `OS_SHFT` (oneshot shift)
2. User presses `SK_COPY`
3. `key_override` sees shift is held, morphs `SK_COPY` → `SK_CUT`
4. `process_semkey()` translates `SK_CUT` to `Cmd+X` (Mac) or `Ctrl+X` (Linux)
5. Oneshot shift is consumed after the keypress

## Current Semantic Keys

| Key       | Mac         | Linux       | Description  |
|-----------|-------------|-------------|--------------|
| `SK_UNDO` | `Cmd+Z`     | `Ctrl+Z`    | Undo         |
| `SK_CUT`  | `Cmd+X`     | `Ctrl+X`    | Cut          |
| `SK_COPY` | `Cmd+C`     | `Ctrl+C`    | Copy         |
| `SK_PSTE` | `Cmd+V`     | `Ctrl+V`    | Paste        |
| `SK_SALL` | `Cmd+A`     | `Ctrl+A`    | Select All   |

## Adding New Semantic Keys

1. Add the keycode to the enum in `keymap.c`:
```c
enum custom_keycodes {
    ...
    SK_NEW_KEY,
    ...
};
```

2. Add the mapping to `semkey_table` in `semantic_keys.h`:
```c
static const semkey_map_t semkey_table[] = {
    ...
    // SK_NEW_KEY
    {G(KC_N), C(KC_N)},  // Mac: Cmd+N, Linux: Ctrl+N
    ...
};
```

3. Update the macros if needed:
```c
#define SK_beg SK_UNDO
#define SK_count (SK_NEW_LAST - SK_beg + 1)
```

## Platform Switching

Toggle between Mac and Linux modes using the `R + V` combo (defined in keymap).

```c
toggle_os_platform();  // Switches between OS_MacOS and OS_Linux
```

## Implementation Notes

- **Timer-less**: No timers involved, works at any typing speed
- **Memory efficient**: Uses direct array lookup, no dynamic allocation
- **Type safe**: Compile-time checks for valid semantic keys
- **Maintainable**: Adding new keys only requires updating the table
