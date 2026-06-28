# Dead Keys Quick Start Guide

## What Are Dead Keys?

Dead keys let you type accented characters by pressing an accent key followed by a letter, just like on traditional keyboards.

## Quick Reference

### How to Use
1. **Hold EXTEND layer** (bottom right thumb key)
2. **Press an accent key** from the top row
3. **Release EXTEND layer**
4. **Type the letter** you want to accent

### Available Accents

| Accent | Position | Example | Result |
|--------|----------|---------|--------|
| **´** Acute | Q position | ´ + e | é |
| **`** Grave | W position | ` + e | è |
| **^** Circumflex | E position | ^ + e | ê |
| **¨** Diaeresis | R position | ¨ + e | ë |
| **~** Tilde | T position | ~ + n | ñ |

### Uppercase
Hold **Shift** while typing the letter:
- ´ + Shift + E = **É**

### All Supported Letters

- **Acute (´)**: a e i o u y → á é í ó ú ý
- **Grave (`)**: a e i o u → à è ì ò ù
- **Circumflex (^)**: a e i o u → â ê î ô û
- **Diaeresis (¨)**: a e i o u y → ä ë ï ö ü ÿ
- **Tilde (~)**: a n o → ã ñ õ

## Common Words Examples

- **café**: c a f `[EXTEND + Q]` e
- **naïve**: n a `[EXTEND + R]` i v e
- **señor**: s e `[EXTEND + T]` n o r
- **crème**: c r `[EXTEND + W]` e m e
- **Zürich**: Shift+Z `[EXTEND + R]` Shift+u r i c h

## Platform Setup

### Linux (Default)
Works out of the box with IBus (installed on most distros).

### macOS
1. Open System Preferences → Keyboard → Input Sources
2. Click **+** to add an input source
3. Select **Unicode Hex Input**
4. Optional: Set a keyboard shortcut to switch between inputs

### Windows
1. Download and install [WinCompose](https://github.com/samhocevar/wincompose)
2. Run WinCompose
3. Dead keys will work automatically

## Troubleshooting

**Accents not appearing?**
- Check your OS setting: Press `E + V + R` (hold E, V, and R together) to see current OS
- Toggle OS: Press `R + V` together to switch between macOS and Linux mode

**Wrong characters?**
- Make sure you've selected the correct OS mode (see above)

**Dead key cancelled?**
- This happens if you press a letter that doesn't support that accent
- Just try again with a compatible letter

## Tips

- Dead keys work with **oneshot modifiers** for uppercase
- Dead keys work across **layer switches**
- You can **cancel** a dead key by pressing an incompatible letter
- Dead keys are **platform-independent** - the keyboard handles everything

## Full Documentation

See [DEAD_KEYS.md](DEAD_KEYS.md) for complete documentation including technical details, architecture, and how to add new accents.
