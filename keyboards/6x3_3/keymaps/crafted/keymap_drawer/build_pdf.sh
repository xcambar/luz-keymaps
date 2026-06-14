#!/bin/bash
# Builds ALL diagram outputs:
#   NN_*.svg / NN_*.png  - color, committed, used by the README ("Direction A" styling)
#   <keymap>.pdf         - color landscape A4 PDF for onscreen use
#   <keymap>_print.pdf   - ADDITIONAL printer-friendly PDF: white keys, black
#                          strokes/text, flat (no paper/shadow), for B&W printers
# Requires: keymap-drawer (via uvx), ImageMagick (convert), ghostscript (gs),
#           Inkscape + the Source Sans 3 font (color diagrams only — see apply_design).

set -euo pipefail
cd "$(dirname "$0")"

# Output filename derived from the keymap directory name
KEYMAP_NAME="$(basename "$(dirname "$(dirname "$(realpath "$0")")")")"

OUTDIR="$(mktemp -d)"
trap 'rm -rf "$OUTDIR"' EXIT

# SVG pattern for held keys (referenced by .key.held CSS as url(#stripes));
# injected into every generated SVG since CSS alone cannot define SVG patterns
STRIPES='<defs><pattern id="stripes" patternUnits="userSpaceOnUse" width="8" height="8" patternTransform="rotate(45)"><rect width="8" height="8" fill="#efefef"/><rect width="4" height="8" fill="#c8c8c8"/></pattern></defs>'

# ImageMagick's SVG coder honors the SVG transform *attribute* but ignores the CSS
# transform *property*. keymap-drawer only emits CSS, so bake the corner-glyph nudge
# (.key.delhint.bl / .key.selhint.br) from the CSS into transform attributes on the
# bl/br elements. Keeps the YAML CSS as the single source of truth. (rsvg-convert is
# not an option: it drops keymap-drawer's <use>-of-nested-<svg> glyphs entirely.)
bake_corner_nudge() {
    python3 - "$1" <<'PY'
import re, sys
p = sys.argv[1]
s = open(p, encoding='utf-8').read()
def css_tf(token):
    m = re.search(r'\.' + token + r'\b[^{]*\{[^}]*?transform:\s*(translate\([^)]*\))', s)
    return re.sub(r'px', '', m.group(1)) if m else None
tfs = {'bl': css_tf('bl'), 'br': css_tf('br')}
def repl(m):
    tag, cls = m.group(0), m.group(1).split()
    if 'transform=' in tag:
        return tag
    for pos in ('bl', 'br'):
        if pos in cls and tfs[pos]:
            return re.sub(r'^(<\w+)', r'\1 transform="%s"' % tfs[pos], tag, count=1)
    return tag
s = re.sub(r'<(?:text|use)\b[^>]*?class="([^"]*)"[^>]*?>', repl, s)
open(p, 'w', encoding='utf-8').write(s)
PY
}

# "Direction A" visual system, applied to the COLOR SVGs only (post-process, so the YAML
# CSS stays the palette source). Humanist sans for labels, mono kept for literal single-
# glyph keycodes ("this is what you type"); warm paper ground; refined caps floating on a
# soft shadow; warm-neutral inactive keys; editorial layer title. Rendered with Inkscape —
# the only backend that does the font + shadow filter + keymap-drawer's <use> glyphs at once.
apply_design() {
    python3 - "$1" <<'PY'
import re, sys
p = sys.argv[1]
s = open(p, encoding='utf-8').read()
SANS = "'Source Sans 3','Source Sans Pro',sans-serif"
MONO = "SFMono-Regular,Consolas,Liberation Mono,Menlo,monospace"

# labels -> humanist sans
s = s.replace('font-family: ' + MONO + ';', 'font-family: ' + SANS + ';')

# literal single-glyph legends stay mono (multi-char labels and multi-line tspans stay sans)
def tap_mono(m):
    open_tag, content, close = m.group(1), m.group(2), m.group(3)
    if re.fullmatch(r'\s*([^<\s]|&[a-zA-Z]+;|&#x?[0-9A-Fa-f]+;)\s*', content):
        open_tag = open_tag.replace('<text ', '<text style="font-family:%s" ' % MONO, 1)
    return open_tag + content + close
s = re.sub(r'(<text\b[^>]*?>)([^<]*)(</text>)', tap_mono, s)

# refined corner radius
s = s.replace('rx="6" ry="6"', 'rx="5" ry="5"')

# warm paper ground + soft keycap shadow filter
defs = ('<defs><filter id="sh" x="-50%" y="-50%" width="200%" height="200%" '
        'color-interpolation-filters="sRGB"><feGaussianBlur stdDeviation="1.6"/></filter></defs>'
        '<rect x="0" y="0" width="100%" height="100%" fill="#faf8f4"/>')
s = re.sub(r'(<svg\b[^>]*>)', r'\1' + defs, s, count=1)

# float shadow behind each ACTIVE key cap (attribute fill -> Inkscape's filter applies; a CSS
# fill would make feDropShadow drop the rect, so we inject a dedicated blurred rect). Offset
# and opacity are tuned so the shadow reads even under the saturated colored category borders
# (a fainter shadow is swallowed by a high-chroma stroke). Disabled keys (none/trans) get NO
# shadow on purpose: their warm-neutral fill recedes into the paper — keep them flat/recessed.
SH = ('<rect x="-25.2" y="-25.5" width="54" height="54" rx="3" ry="3" '
      'fill="#3a2f25" opacity="0.16" filter="url(#sh)"/>')
def float_cap(m):
    rect, cls = m.group(1), m.group(2).split()
    if 'none' in cls or 'trans' in cls:
        return rect
    return SH + rect
s = re.sub(r'(<rect rx="5" ry="5"[^>]*class="(key[^"]*)"/>)', float_cap, s)

# warm-neutral inactive keys (unused + transparent) instead of cool white
s = s.replace('.key.none rect { fill: white; }',
              '.key.none rect, .key.trans rect { fill: #f4f0ea; }')

# editorial layer title
s = s.replace('text.label {\n    font-weight: bold;',
              "text.label {\n    font-weight: 600;\n    font-size: 20px;\n    letter-spacing: 0.08em;\n    fill: #5a5048;")

open(p, 'w', encoding='utf-8').write(s)
PY
}

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

    # Color version (committed; referenced by the README and the color PDF)
    uvx --from keymap-drawer keymap draw "$yml" > "$name.svg"
    sed -i "s|</svg>|$STRIPES</svg>|" "$name.svg"
    bake_corner_nudge "$name.svg"
    apply_design "$name.svg"
    inkscape "$name.svg" --export-type=png --export-filename="$name.png" --export-dpi=150 >/dev/null 2>&1
    cp "$name.png" "$OUTDIR/color_$name.png"

    # Print variant (temp files only; ends up solely inside the print PDF)
    awk '/^draw_config:/{exit} {print}' "$yml" > "$OUTDIR/$name.print.yml"
    printf '%s\n' "$PRINT_CONFIG" >> "$OUTDIR/$name.print.yml"
    uvx --from keymap-drawer keymap draw "$OUTDIR/$name.print.yml" > "$OUTDIR/$name.print.svg"
    sed -i "s|</svg>|$STRIPES</svg>|" "$OUTDIR/$name.print.svg"
    bake_corner_nudge "$OUTDIR/$name.print.svg"
    convert -density 150 "$OUTDIR/$name.print.svg" "$OUTDIR/print_$name.png"

    echo "  $yml -> $name.svg -> $name.png (+ print variant)"
done

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
