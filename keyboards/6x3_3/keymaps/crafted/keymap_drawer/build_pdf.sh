#!/bin/bash
# Builds ALL diagram outputs:
#   NN_*.svg / NN_*.png  - color, committed, used by the README (unchanged)
#   <keymap>.pdf         - color landscape A4 PDF for onscreen use (unchanged)
#   <keymap>_print.pdf   - ADDITIONAL printer-friendly PDF: white keys, black
#                          strokes/text, for B&W printers
# Requires: keymap-drawer (via uvx), ImageMagick (convert), ghostscript (gs)

set -euo pipefail
cd "$(dirname "$0")"

# Output filename derived from the keymap directory name
KEYMAP_NAME="$(basename "$(dirname "$(dirname "$(realpath "$0")")")")"

OUTDIR="$(mktemp -d)"
trap 'rm -rf "$OUTDIR"' EXIT

# SVG pattern for held keys (referenced by .key.held CSS as url(#stripes));
# injected into every generated SVG since CSS alone cannot define SVG patterns
STRIPES='<defs><pattern id="stripes" patternUnits="userSpaceOnUse" width="8" height="8" patternTransform="rotate(45)"><rect width="8" height="8" fill="#efefef"/><rect width="4" height="8" fill="#c8c8c8"/></pattern></defs>'

# High-contrast draw_config substituted for the embedded one, for the print PDF only
PRINT_CONFIG=$(cat <<'EOF'
draw_config:
    key_h: 60
    append_colon_to_layer_header: false
    svg_extra_style: |
        .key rect { fill: white; stroke: black; stroke-width: 1.2; }
        .key :not(rect) { fill: black; }
        .key.trans :not(rect) { fill: #777777; }
        .key.modtap.hold { font-weight: bold; }
        .key.delhold :not(rect) { fill: #d6453a; }
        .key.delhint.hold { fill: #d6453a; font-size: 17px; }
        .key.held rect { fill: url(#stripes); stroke: #555555; }
        .combo rect { fill: white; stroke: black; }
EOF
)

echo "Generating SVGs and PNGs..."
for yml in [0-9]*.yml; do
    name="${yml%.yml}"

    # Color version (committed; referenced by the README and the color PDF)
    uvx --from keymap-drawer keymap draw "$yml" > "$name.svg"
    sed -i "s|</svg>|$STRIPES</svg>|" "$name.svg"
    convert -density 150 "$name.svg" "$name.png"
    cp "$name.png" "$OUTDIR/color_$name.png"

    # Print variant (temp files only; ends up solely inside the print PDF)
    awk '/^draw_config:/{exit} {print}' "$yml" > "$OUTDIR/$name.print.yml"
    printf '%s\n' "$PRINT_CONFIG" >> "$OUTDIR/$name.print.yml"
    uvx --from keymap-drawer keymap draw "$OUTDIR/$name.print.yml" > "$OUTDIR/$name.print.svg"
    sed -i "s|</svg>|$STRIPES</svg>|" "$OUTDIR/$name.print.svg"
    convert -density 150 "$OUTDIR/$name.print.svg" "$OUTDIR/print_$name.png"

    echo "  $yml -> $name.svg -> $name.png (+ print variant)"
done

# compose_pdf <png-prefix> <output-pdf>
compose_pdf() {
    local prefix="$1" out="$2"
    local pages=()
    # A4 landscape at 150 DPI: 1754x1240 pixels
    for png in "$OUTDIR/${prefix}_"*.png; do
        convert -size 1754x1240 xc:white \
            "$png" -gravity center -composite \
            "${png%.png}_a4.png"
        pages+=("${png%.png}_a4.png")
    done
    convert "${pages[@]}" "$OUTDIR/${prefix}_pages.pdf"
    gs -sDEVICE=pdfwrite -dNOPAUSE -dBATCH -dQUIET \
       -dDEVICEWIDTHPOINTS=842 -dDEVICEHEIGHTPOINTS=595 \
       -dFIXEDMEDIA -dPDFFitPage \
       -sOutputFile="$out" \
       "$OUTDIR/${prefix}_pages.pdf"
    echo "Done: $out"
}

echo "Composing PDFs..."
compose_pdf color "${KEYMAP_NAME}.pdf"
compose_pdf print "${KEYMAP_NAME}_print.pdf"
