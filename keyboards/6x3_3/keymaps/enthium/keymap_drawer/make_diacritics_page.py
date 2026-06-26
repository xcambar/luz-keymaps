#!/usr/bin/env python3
"""Generate 05_DIACRITICS.svg — a reference page for the Compose / diacritics feature.

Like make_modes_page.py, this is NOT a keymap-drawer layer; it's a hand-built table
page that mirrors the "Compose & diacritics" section of the README, styled to match
the Direction A look of the layer diagrams (warm paper, Source Sans 3, floating
shadows). build_pdf.sh runs this and rasterises the result into the PDFs as the page
after 04_MODES. Edit the data tables below to change the page.

The amber accent ties the page to the Compose key, which renders as `type: state`
(fill #fdebd9 / stroke #dd9d57) on the BASE diagram.
"""

# ── Direction A palette (kept in sync with the layer diagrams / make_modes_page.py) ──
PAPER  = "#faf8f4"
INK    = "#36322e"   # default text
MUTED  = "#8a8076"   # secondary text
TITLE  = "#5a5048"
SANS   = "'Source Sans 3','Source Sans Pro',sans-serif"
MONO   = "SFMono-Regular,Consolas,Liberation Mono,Menlo,monospace"

# Compose accent (matches the BASE Compose key's `state` tint)
A_FILL, A_STROKE, A_INK = "#fdebd9", "#dd9d57", "#a8671c"

# ── data (single source for both the SVG and the README markdown twin) ──────────
# Dead keys: arm Compose, press this key, then a base letter to combine.
#   (key, mark, name, example base letter, example result)
DEAD = [
    ("e", "´", "acute",      "e", "é"),
    ("a", "`", "grave",      "e", "è"),
    ("u", "¨", "diaeresis",  "e", "ë"),
    ("o", "ˆ", "circumflex", "e", "ê"),
]
# Direct characters: arm Compose, press this key, done.  (key, char, name)
DIRECT = [
    ("c", "ç", ""),
    ("n", "ñ", ""),
    ("w", "€", "euro"),
]

# formatters shared by the SVG rows and the markdown twin
def _dead_produces(mark, name):     return f"{mark}  {name}"
def _dead_example(key, base, res):  return f"Shift+Space   {key}   {base}   →   {res}"
def _dir_produces(char, name):      return f"{char}  {name}" if name else char
def _dir_example(key, char):        return f"Shift+Space   {key}   →   {char}"

# ── markdown twin (keeps the README "Reference tables" block in sync) ──────────
# `make_diacritics_page.py --md` prints just the table; build_pdf.sh injects it
# between the <!-- BEGIN/END DIACRITICS TABLE --> markers in the README.
import sys
if "--md" in sys.argv:
    md = ["| Key | Produces | Example |", "|-----|----------|---------|"]
    for key, mark, name, base, res in DEAD:
        m = "\\`" if mark == "`" else mark  # bare backtick would open inline code in a cell
        md.append(f"| `{key}` | {m} {name} (dead key) | `Shift+Space`, `{key}`, `{base}` → {res} |")
    for key, char, name in DIRECT:
        prod = f"{char} ({name})" if name else char
        md.append(f"| `{key}` | {prod} | `Shift+Space`, `{key}` → {char} |")
    footnote = ("Armed from the **base layer** with Shift + Space. Dead keys wait for a base "
                "letter, so the same accent works on any vowel; any unlisted key cancels.")
    print("\n".join([*md, "", footnote]))
    sys.exit()

# ── layout geometry ───────────────────────────────────────────────────────────
W, MX = 980, 40
CW = W - 2 * MX                       # content width = 900
CARD_TOP, CARD_H = 98, 66
TAB_TOP = CARD_TOP + CARD_H + 30      # table top
KEYW = 96                             # key column width
PRODW = 240                           # "produces" column width
HEADH, ROWH, GRPH = 30, 30, 30        # header / data row / group-label row
DOTCOL = "#b9b0a6"

# vertical cursor walks down as we emit the table
def table_height():
    return HEADH + GRPH + len(DEAD) * ROWH + GRPH + len(DIRECT) * ROWH

H = int(TAB_TOP + table_height() + 70)

s = []
def add(x): s.append(x)

add(f'<svg width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
    f'xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">')
add('<defs><filter id="sh" x="-50%" y="-50%" width="200%" height="200%" '
    'color-interpolation-filters="sRGB"><feGaussianBlur stdDeviation="2.2"/></filter></defs>')
add(f'<rect x="0" y="0" width="100%" height="100%" fill="{PAPER}"/>')
add(f'<style>text{{font-family:{SANS};fill:{INK};}}</style>')

# title + intro
add(f'<text x="{MX}" y="50" font-size="26" font-weight="600" '
    f'letter-spacing="1.2" fill="{TITLE}">Compose &amp; diacritics</text>')
add(f'<text x="{MX}" y="78" font-size="13.5" fill="{MUTED}">'
    'Press Shift + Space on the base layer to arm Compose, then press one key.</text>')

# ── trigger card (full width: Arm | Cancel) ───────────────────────────────────
add(f'<rect x="{MX+1.5}" y="{CARD_TOP+2.5}" width="{CW}" height="{CARD_H}" '
    f'rx="8" ry="8" fill="#3a2f25" opacity="0.10" filter="url(#sh)"/>')
add(f'<rect x="{MX}" y="{CARD_TOP}" width="{CW}" height="{CARD_H}" '
    f'rx="8" ry="8" fill="{A_FILL}" stroke="{A_STROKE}" stroke-width="1.3"/>')
MIDX = MX + CW * 0.52
add(f'<line x1="{MIDX:.1f}" y1="{CARD_TOP+14}" x2="{MIDX:.1f}" y2="{CARD_TOP+CARD_H-14}" '
    f'stroke="{A_STROKE}" stroke-width="1" opacity="0.5"/>')
# Arm
add(f'<text x="{MX+18}" y="{CARD_TOP+28}" font-size="13" font-weight="700" '
    f'fill="{A_INK}">Arm</text>')
add(f'<text x="{MX+18}" y="{CARD_TOP+50}" font-size="14.5" fill="{INK}">'
    '<tspan font-weight="600">Shift + Space</tspan>'
    '<tspan dx="12" fill="' + MUTED + '" font-size="12.5">from the base layer</tspan></text>')
# Cancel
add(f'<text x="{MIDX+18:.1f}" y="{CARD_TOP+28}" font-size="13" font-weight="700" '
    f'fill="{A_INK}">Cancel</text>')
add(f'<text x="{MIDX+18:.1f}" y="{CARD_TOP+50}" font-size="14.5" fill="{INK}">'
    '<tspan font-weight="600">Esc</tspan>'
    '<tspan dx="12" fill="' + MUTED + '" font-size="12.5">any other key types as usual</tspan></text>')

# ── table ─────────────────────────────────────────────────────────────────────
key_cx   = MX + KEYW / 2
prod_x   = MX + KEYW + 16
ex_x     = MX + KEYW + PRODW + 16

# header
hy = TAB_TOP + HEADH - 9
add(f'<text x="{key_cx}" y="{hy}" font-size="12" font-weight="600" text-anchor="middle" '
    f'fill="{MUTED}">Key</text>')
add(f'<text x="{prod_x}" y="{hy}" font-size="12" font-weight="600" fill="{MUTED}">Produces</text>')
add(f'<text x="{ex_x}" y="{hy}" font-size="12" font-weight="600" fill="{MUTED}">Example</text>')
add(f'<rect x="{MX}" y="{TAB_TOP+HEADH-4}" width="{CW}" height="2" rx="1" fill="{A_STROKE}"/>')

# tinted key column behind all data rows
data_top = TAB_TOP + HEADH
data_h = GRPH + len(DEAD) * ROWH + GRPH + len(DIRECT) * ROWH
add(f'<rect x="{MX}" y="{data_top}" width="{KEYW}" height="{data_h}" rx="4" ry="4" fill="{A_FILL}"/>')

def group_label(y, text, note):
    ty = y + GRPH / 2 + 4
    add(f'<text x="{prod_x}" y="{ty:.1f}" font-size="11.5" font-weight="700" '
        f'letter-spacing="0.6" fill="{A_INK}">{text}'
        f'<tspan dx="8" fill="{MUTED}" font-weight="400" letter-spacing="0">— {note}</tspan></text>')
    # small accent underline beneath the title (approx title width) to separate sections
    underw = len(text) * 7.2
    add(f'<rect x="{prod_x}" y="{ty + 6:.1f}" width="{underw:.1f}" height="2" rx="1" fill="{A_STROKE}"/>')

def data_row(y, key, produces, example, zebra):
    if zebra:
        add(f'<rect x="{MX+KEYW}" y="{y}" width="{CW-KEYW}" height="{ROWH}" fill="#f1ece4"/>')
    ty = y + ROWH / 2 + 5
    add(f'<text x="{key_cx}" y="{ty:.1f}" font-size="14" font-weight="600" '
        f'style="font-family:{MONO}" text-anchor="middle" fill="{INK}">{key}</text>')
    add(f'<text x="{prod_x}" y="{ty:.1f}" font-size="13.5" fill="{INK}">{produces}</text>')
    add(f'<text x="{ex_x}" y="{ty:.1f}" font-size="12.5" style="font-family:{MONO}" '
        f'fill="{MUTED}">{example}</text>')

y = data_top
group_label(y, "DEAD KEYS", "press, then type the base letter"); y += GRPH
for i, (key, mark, name, base, res) in enumerate(DEAD):
    data_row(y, key, _dead_produces(mark, name), _dead_example(key, base, res), i % 2 == 1); y += ROWH
group_label(y, "DIRECT", "prints the character immediately"); y += GRPH
for i, (key, char, name) in enumerate(DIRECT):
    data_row(y, key, _dir_produces(char, name), _dir_example(key, char), i % 2 == 1); y += ROWH

# legend
ly = y + 28
add(f'<text x="{MX}" y="{ly}" font-size="11.5" fill="{MUTED}">'
    'Dead keys wait for a base letter, so the same accent works on any vowel '
    '(é á í ó ú · è à ì ò ù …). Accents are OS-aware (macOS &amp; Linux).</text>')

add('</svg>')
print("\n".join(s))
