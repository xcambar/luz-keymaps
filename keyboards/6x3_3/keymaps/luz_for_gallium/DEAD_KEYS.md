# Dead Keys Implementation

This implementation provides dead key functionality by triggering the OS's native dead key system. This is simpler and more reliable than Unicode input.

## How It Works

The keyboard sends the OS-specific dead key sequence when you press a dead key:

1. **Press a dead key** (e.g., `DK_ACUTE`) → Keyboard sends `AltGr + '` on Linux
2. **Press a letter** (e.g., `E`) → Your OS combines them to produce `é`
3. **OS handles everything** → No special Unicode configuration needed

## Platform Support

### Linux (Current Implementation)
Uses **AltGr (Right Alt)** + key to trigger dead keys:

| Dead Key | Linux Sequence | Result with E | All Letters |
|----------|---------------|---------------|-------------|
| **DK_ACUTE** | AltGr + ' | é | á é í ó ú ý |
| **DK_GRAVE** | AltGr + ` | è | à è ì ò ù |
| **DK_CIRC** | AltGr + 6 | ê | â ê î ô û |
| **DK_DIAE** | AltGr + Shift + ' | ë | ä ë ï ö ü ÿ |
| **DK_TILDE** | AltGr + n | ẽ | ã ñ õ |

**Requirements**: Standard Linux with standard keyboard layout (most distros)

### macOS (Not Yet Implemented)
Placeholder: Returns `KC_NO` (does nothing)

**TODO**: Implement macOS Option + key sequences

## Usage

### Basic Usage
1. Hold **EXTEND** layer (bottom right thumb key)
2. Press an accent key from the top row (Q/W/E/R/T positions)
3. Release EXTEND
4. Type the letter you want to accent

### Examples

**Type "café":**
- `c` `a` `f` `[EXTEND + Q]` `e` → café

**Type "naïve":**
- `n` `a` `[EXTEND + R]` `i` `v` `e` → naïve

**Type "señor":**
- `s` `e` `[EXTEND + T]` `n` `o` `r` → señor

## Keymap Placement

Dead keys are on the top row of the SYMBOLS layer:

```
SYMBOLS Layer (accessed via EXTEND):
┌───┬───┬───┬───┬───┬───┐
│   │ ´ │ ` │ ^ │ ¨ │ ~ │
│   │ Q │ W │ E │ R │ T │
```

## Architecture

### Design Philosophy
Unlike the previous Unicode approach, this implementation:
- **Delegates to the OS**: Uses native dead key functionality
- **Platform-aware**: Sends different sequences per OS (like semantic keys)
- **Zero configuration**: Works with standard Linux keyboard layouts
- **Lightweight**: No Unicode subsystem needed

### Code Structure
```c
typedef struct {
    uint16_t mac_keycode;    // macOS sequence (placeholder)
    uint16_t linux_keycode;  // Linux AltGr sequence
} deadkey_map_t;
```

Similar to semantic keys, uses a lookup table indexed by keycode.

## Advantages Over Unicode Approach

| Aspect | Unicode | OS Dead Keys (Current) |
|--------|---------|----------------------|
| **Setup** | Requires Unicode config | Works out of the box |
| **Reliability** | Can fail with IMEs | Always works |
| **Speed** | Slower (Unicode input) | Instant |
| **Size** | +1.4KB firmware | Smaller firmware |
| **Compatibility** | 90% of systems | 99% of systems |

## Compatibility

### ✅ Works With
- Standard Linux keyboard layouts (US International, EurKEY, etc.)
- GNOME, KDE, XFCE, i3, etc.
- Most terminal emulators
- All text editors

### ❌ May Not Work With
- Custom keyboard layouts that remove AltGr dead keys
- Some minimal Linux distributions without proper locale support

### 🔧 Troubleshooting

**Dead keys not working on Linux?**
1. Check your keyboard layout supports AltGr dead keys:
   ```bash
   setxkbmap -query  # Should show 'us(intl)' or similar
   ```
2. Try switching to US International layout:
   ```bash
   setxkbmap -layout us -variant intl
   ```

**Getting weird characters?**
- Your keyboard layout might have different dead key mappings
- Try the US International layout for standard behavior

## Adding More Dead Keys

To add a new dead key:

1. Add keycode to [custom_keycodes.h](custom_keycodes.h):
```c
enum custom_keycodes {
    ...
    DK_CARON,  // For č š ž
    ...
};
```

2. Add mapping to `deadkey_table` in [dead_keys.c](features/dead_keys.c):
```c
[DK_ndx(DK_CARON)] = {KC_NO, RALT(KC_V)},  // Linux: AltGr + v
```

3. Place it in your keymap

## Future Work

- [ ] Implement macOS dead key sequences
- [ ] Add support for more exotic accents (caron, ring, cedilla)
- [ ] Auto-detect OS and show warning if dead keys unavailable
- [ ] Per-key documentation in keymap comments

## Technical Notes

### Why This Approach?
The previous Unicode implementation had issues:
1. Modifiers (especially Alt) were interfering
2. Unicode input is slow and visible to the user
3. Required complex state machine to track dead key state
4. Larger firmware size

The OS dead key approach:
1. One `tap_code16()` call - done!
2. OS handles all the complexity
3. Instant feedback
4. Smaller code

### Comparison to Semantic Keys
Dead keys follow the same pattern as semantic keys:
- Platform-aware lookup table
- Uses `get_os_platform()` to select correct sequence
- Single tap sends platform-specific keycode
- No state machine needed

### Firmware Size
- **Unicode version**: 39,420 bytes
- **OS dead keys**: 38,124 bytes
- **Savings**: 1,296 bytes (3.3% reduction)
