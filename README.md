# Luz - a framework to build complete keymaps for 3x6+3 split keyboards

![](./header.jpeg)

[**Luz**](./LUZ.md) is a framework of shared conventions — layer model, mod placement,
symbols, Compose, navigation — that stay identical across layouts, so muscle memory transfers
while each layout keeps its own alphas. It ships as layout-specific *variants*. Start with
[`LUZ.md`](./LUZ.md) for the guided tour, or [`LUZ.spec.md`](./LUZ.spec.md) for the exact contract.

## The example variants

| Keymap | What it is | Details |
|--------|------------|---------|
| **`crafted`** | My daily driver — Luz on the **Gallium East** layout | [crafted README](./keyboards/6x3_3/keymaps/crafted/README.md) |
| **`enthium`** *(WIP)* | Luz on the **Enthium** layout | [enthium README](./keyboards/6x3_3/keymaps/enthium/README.md) |
| **`zen`** | A minimal QWERTY fallback (3 layers, no custom features) | — |

> [!IMPORTANT]
> By design, the top-left and top-right keys are never used — they're too uncomfortable to reach.

## Building

I have run these keymaps on:

| MCU | Example keyboard |
|-----|------------------|
| STM32 | **Kaly42** (`kaly/kaly42`) |
| RP2040 | **Cantor Pro v3** (`42keebs/cantor_pro/v3/left`) |

Build a single target, or all of them at once:

```bash
qmk compile -kb 42keebs/cantor_pro/v3/left -km crafted
qmk userspace-compile
```

## Inspiration

- **[HandsDown](https://sites.google.com/alanreiser.com/handsdown)** — semantic, platform-aware editing commands
- **[Gallium](https://github.com/GalileoBlues/Gallium) East** & **[Enthium](https://github.com/sunaku/enthium)** — the alpha layouts
- **Pascal Getreuer's QMK work** — Chordal Hold, Caps Word, and the wider tap-hold tuning vocabulary
- **Callum-style oneshot modifiers** — an early influence, since fully replaced by layer-scoped latches and plain momentary mods
- **[keymap-drawer](https://github.com/caksoylar/keymap-drawer)** — the layer diagrams

## License

GPL-2.0-or-later (following QMK licensing)
