# Variants

A **variant** is one alpha layout in the Luz frame. Each page below covers what the
variant chooses within [what a variant may change](../README.md#what-a-variant-may-change),
with the full diagrams of its layers and a printable PDF.

| Variant | Letters | Page | QMK keymap |
|---------|---------|------|------------|
| **Luz for Gallium** | Gallium East | [gallium](gallium/README.md) | [`luz_for_gallium`](../../packages/qmk/keyboards/6x3_3/keymaps/luz_for_gallium/) |
| **Luz for Enthium** | Enthium | [enthium](enthium/README.md) | [`luz_for_enthium`](../../packages/qmk/keyboards/6x3_3/keymaps/luz_for_enthium/) |
| **Luz for Colemak-DH** | Colemak Mod-DH (matrix) | [colemak_dh](colemak_dh/README.md) | [`luz_for_colemak_dh`](../../packages/qmk/keyboards/6x3_3/keymaps/luz_for_colemak_dh/) |
| **Luz for QWERTY** | QWERTY | [qwerty](qwerty/README.md) | [`luz_for_qwerty`](../../packages/qmk/keyboards/6x3_3/keymaps/luz_for_qwerty/) |

Each variant's diagrams are built from the `*.yml` files in its directory, from the
repository root:

```bash
spec/diagrams/build_pdf.sh <variant>    # e.g. spec/diagrams/build_pdf.sh gallium
```
