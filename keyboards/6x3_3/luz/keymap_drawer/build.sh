#!/bin/bash
# Render the generic Luz BASE illustration (BASE.yml -> BASE.svg) in the same "Direction A"
# look as the per-variant diagrams. This is a Luz-level asset (alphas left blank), so it lives
# here rather than in a keymap's keymap_drawer/. The apply_design step is shared with
# build_pdf.sh via design.py. SVG only — no PNG/PDF/print.
# Requires: keymap-drawer (via uvx).
set -euo pipefail
cd "$(dirname "$0")"

STRIPES='<defs><pattern id="stripes" patternUnits="userSpaceOnUse" width="8" height="8" patternTransform="rotate(45)"><rect width="8" height="8" fill="#efefef"/><rect width="4" height="8" fill="#c8c8c8"/></pattern></defs>'

# "Direction A" visual system, shared with build_pdf.sh
apply_design() { python3 design.py style "$1"; }

for yml in *.yml; do
    name="${yml%.yml}"
    uvx --from keymap-drawer keymap draw "$yml" > "$name.svg"
    sed -i "s|</svg>|$STRIPES</svg>|" "$name.svg"
    apply_design "$name.svg"
    echo "  $yml -> $name.svg"
done
