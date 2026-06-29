#!/bin/bash
# Render the generic Luz BASE illustration (BASE.yml -> BASE.svg) in the same "Direction A"
# look as the per-variant diagrams. This is a Luz-level asset (alphas left blank), so it lives
# here rather than in a keymap's keymap_drawer/. The apply_design step below is kept in sync
# with keyboards/6x3_3/keymaps/*/keymap_drawer/build_pdf.sh. SVG only — no PNG/PDF/print.
# Requires: keymap-drawer (via uvx).
set -euo pipefail
cd "$(dirname "$0")"

STRIPES='<defs><pattern id="stripes" patternUnits="userSpaceOnUse" width="8" height="8" patternTransform="rotate(45)"><rect width="8" height="8" fill="#efefef"/><rect width="4" height="8" fill="#c8c8c8"/></pattern></defs>'

# "Direction A" visual system (copied from build_pdf.sh's apply_design; keep in sync).
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

# float shadow behind each ACTIVE key cap (none/trans keys stay flat/recessed)
SH = ('<rect x="-25.2" y="-25.5" width="54" height="54" rx="3" ry="3" '
      'fill="#3a2f25" opacity="0.16" filter="url(#sh)"/>')
def float_cap(m):
    rect, cls = m.group(1), m.group(2).split()
    if 'none' in cls or 'trans' in cls:
        return rect
    return SH + rect
s = re.sub(r'(<rect rx="5" ry="5"[^>]*class="(key[^"]*)"/>)', float_cap, s)

# warm-neutral inactive keys
s = s.replace('.key.none rect { fill: white; }',
              '.key.none rect, .key.trans rect { fill: #f4f0ea; }')

# editorial layer title
s = s.replace('text.label {\n    font-weight: bold;',
              "text.label {\n    font-weight: 600;\n    font-size: 20px;\n    letter-spacing: 0.08em;\n    fill: #5a5048;")

open(p, 'w', encoding='utf-8').write(s)
PY
}

for yml in *.yml; do
    name="${yml%.yml}"
    uvx --from keymap-drawer keymap draw "$yml" > "$name.svg"
    sed -i "s|</svg>|$STRIPES</svg>|" "$name.svg"
    apply_design "$name.svg"
    echo "  $yml -> $name.svg"
done
