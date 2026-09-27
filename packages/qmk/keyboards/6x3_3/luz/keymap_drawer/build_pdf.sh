#!/bin/bash
# Builds ALL diagram outputs:
#   NN_*.svg             - color, committed, used by the README ("Direction A" styling)
#   <keymap>.pdf         - color landscape A4 PDF for onscreen use
#   (PNGs are intermediate only: rendered into the temp dir for the PDFs, not committed)
#   <keymap>_print.pdf   - ADDITIONAL printer-friendly PDF: white keys, black
#                          strokes/text, flat (no paper/shadow), for B&W printers
# Requires: keymap-drawer (via uvx), ImageMagick (convert), ghostscript (gs),
#           Inkscape + the Source Sans 3 font (color diagrams only — see design.py).
#
# Usage: build_pdf.sh <keymap>     e.g. build_pdf.sh luz_for_gallium
# Works on keyboards/6x3_3/keymaps/<keymap>/keymap_drawer/, from any directory.

set -euo pipefail
LUZ_DRAW="$(dirname "$(realpath "$0")")"
KEYMAP_NAME="${1:?usage: build_pdf.sh <keymap>, e.g. luz_for_gallium}"
cd "$LUZ_DRAW/../../keymaps/$KEYMAP_NAME/keymap_drawer"

OUTDIR="$(mktemp -d)"
trap 'rm -rf "$OUTDIR"' EXIT

# SVG pattern for held keys (referenced by .key.held CSS as url(#stripes));
# injected into every generated SVG since CSS alone cannot define SVG patterns
STRIPES='<defs><pattern id="stripes" patternUnits="userSpaceOnUse" width="8" height="8" patternTransform="rotate(45)"><rect width="8" height="8" fill="#efefef"/><rect width="4" height="8" fill="#c8c8c8"/></pattern></defs>'

# SVG post-processing (corner-glyph nudge, "Direction A" look) lives in design.py,
# shared with build.sh.
bake_corner_nudge() { python3 "$LUZ_DRAW/design.py" nudge "$1"; }
apply_design() { python3 "$LUZ_DRAW/design.py" style "$1"; }

# High-contrast draw_config substituted for the embedded one, for the print PDF only
PRINT_CONFIG=$(cat <<'EOF'
draw_config:
    key_h: 60
    glyph_shifted_size: 16
    append_colon_to_layer_header: false
    svg_extra_style: |
        /* B/W print: white keys, black legends; meaning carried by shape + position.
           Avoid the :not() pseudo-class — librsvg (the PNG backend) ignores it. */
        .key rect { fill: white; stroke: black; stroke-width: 1.2; }
        .key.trans.tap { fill: #777777; }
        .key.modtap.hold { font-weight: bold; }
        .key.mode.shifted { font-style: italic; }
        .key.delhint.bl { font-size: 15px; transform: translate(3px, -3px); }
        .key.selhint.br { transform: translate(-3px, -3px); }
        .key.held rect { fill: url(#stripes); stroke: #555555; }
        .combo rect { fill: white; stroke: black; }
EOF
)

echo "Generating SVGs and PNGs..."
for yml in [0-9]*.yml; do
    name="${yml%.yml}"

    # Color SVG is committed (referenced by the README); its PNG is a temp-only
    # intermediate consumed by the color PDF, so render it straight into OUTDIR.
    uvx --from keymap-drawer keymap draw "$yml" > "$name.svg"
    sed -i "s|</svg>|$STRIPES</svg>|" "$name.svg"
    bake_corner_nudge "$name.svg"
    apply_design "$name.svg"
    inkscape "$name.svg" --export-type=png --export-filename="$OUTDIR/color_$name.png" --export-dpi=150 >/dev/null 2>&1

    # Print variant (temp files only; ends up solely inside the print PDF)
    awk '/^draw_config:/{exit} {print}' "$yml" > "$OUTDIR/$name.print.yml"
    printf '%s\n' "$PRINT_CONFIG" >> "$OUTDIR/$name.print.yml"
    uvx --from keymap-drawer keymap draw "$OUTDIR/$name.print.yml" > "$OUTDIR/$name.print.svg"
    sed -i "s|</svg>|$STRIPES</svg>|" "$OUTDIR/$name.print.svg"
    bake_corner_nudge "$OUTDIR/$name.print.svg"
    convert -density 150 "$OUTDIR/$name.print.svg" "$OUTDIR/print_$name.png"

    echo "  $yml -> $name.svg (+ temp PNG and print variant for the PDFs)"
done

# Modes reference page: a hand-built table (not a keymap-drawer layer), generated in
# the Direction A look and rasterised as the page after 03_ADJUST. The committed SVG
# already carries its final styling, so it skips bake_corner_nudge/apply_design. The
# print PDF gets a desaturated copy.
echo "Generating modes reference page..."
python3 "$LUZ_DRAW/make_modes_page.py" > 04_MODES.svg
inkscape 04_MODES.svg --export-type=png --export-filename="$OUTDIR/color_04_MODES.png" --export-dpi=150 >/dev/null 2>&1
convert "$OUTDIR/color_04_MODES.png" -colorspace Gray "$OUTDIR/print_04_MODES.png"
echo "  make_modes_page.py -> 04_MODES.svg (+ PNGs for the PDFs)"

# Diacritics reference page: same hand-built treatment as the modes page, rasterised
# as the page after 04_MODES. Pre-styled, so it skips bake_corner_nudge/apply_design.
echo "Generating diacritics reference page..."
python3 "$LUZ_DRAW/make_diacritics_page.py" > 05_DIACRITICS.svg
inkscape 05_DIACRITICS.svg --export-type=png --export-filename="$OUTDIR/color_05_DIACRITICS.png" --export-dpi=150 >/dev/null 2>&1
convert "$OUTDIR/color_05_DIACRITICS.png" -colorspace Gray "$OUTDIR/print_05_DIACRITICS.png"
echo "  make_diacritics_page.py -> 05_DIACRITICS.svg (+ PNGs for the PDFs)"

# Keep the README "Reference tables" block in sync with the SAME data that drives the SVG
# pages: each generator emits its table as markdown with --md, injected between the
# <!-- BEGIN/END … TABLE --> markers so the searchable text twin never drifts from the diagram.
README="../README.md"
sync_readme_table() {   # $1 = marker tag, $2 = markdown file
    python3 - "$README" "$1" "$2" <<'PY'
import re, sys
readme, tag, src = sys.argv[1:4]
md = open(src, encoding='utf-8').read().strip()
s = open(readme, encoding='utf-8').read()
pat = re.compile(re.escape(f'<!-- BEGIN {tag} -->') + r'.*?' + re.escape(f'<!-- END {tag} -->'), re.DOTALL)
repl = f'<!-- BEGIN {tag} -->\n\n{md}\n\n<!-- END {tag} -->'
s2, n = pat.subn(lambda m: repl, s)
if n != 1:
    sys.exit(f"sync_readme_table: expected 1 '{tag}' marker pair, found {n}")
open(readme, 'w', encoding='utf-8').write(s2)
PY
}
echo "Syncing README reference tables..."
python3 "$LUZ_DRAW/make_modes_page.py" --md > "$OUTDIR/nav_table.md"
python3 "$LUZ_DRAW/make_diacritics_page.py" --md > "$OUTDIR/diac_table.md"
sync_readme_table "NAV TABLE" "$OUTDIR/nav_table.md"
sync_readme_table "DIACRITICS TABLE" "$OUTDIR/diac_table.md"
echo "  README NAV TABLE + DIACRITICS TABLE updated"

# compose_pdf <png-prefix> <output-pdf> <page-bg>
compose_pdf() {
    local prefix="$1" out="$2" bg="$3"
    local pages=()
    # A4 landscape at 150 DPI: 1754x1240 pixels
    for png in "$OUTDIR/${prefix}_"*.png; do
        convert -size 1754x1240 "xc:$bg" \
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
compose_pdf color "${KEYMAP_NAME}.pdf" "#faf8f4"
compose_pdf print "${KEYMAP_NAME}_print.pdf" white
