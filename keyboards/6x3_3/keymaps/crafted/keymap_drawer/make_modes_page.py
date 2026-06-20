#!/usr/bin/env python3
"""Generate 04_MODES.svg — a reference page for the FAVS navigation modes.

This is NOT a keymap-drawer layer; it's a hand-built table page that mirrors the
"Navigation modes" section of the README, styled to match the Direction A look of
the layer diagrams (warm paper, Source Sans 3, the category palette, floating
shadows). build_pdf.sh runs this and rasterises the result into the PDFs as the
page after 03_ADJUST. Edit the data tables below to change the page.
"""

# ── Direction A palette (kept in sync with 02_FAVS.yml's svg_extra_style) ──────
PAPER  = "#faf8f4"
INK    = "#36322e"   # default text
MUTED  = "#8a8076"   # secondary text
TITLE  = "#5a5048"
SANS   = "'Source Sans 3','Source Sans Pro',sans-serif"
# Single-glyph keycaps render in mono (as the layer diagrams do via apply_design): the
# sans stack lacks ⏮/⏭ and falls back to garbage (renders "Z"/"Y"); the mono stack has them.
MONO   = "SFMono-Regular,Consolas,Liberation Mono,Menlo,monospace"

# Per-mode accent colours (fill = key tint, stroke = border, ink = legend text)
MODES = [
    {"name": "Navigation", "tag": "base",  "fill": "#dbe9f8", "stroke": "#6b97c9", "ink": "#2f5a86",
     "engage": "Hold left inner thumb", "exit": "Release (or Layer Lock)"},
    {"name": "Select",     "tag": "Sl⊙",   "fill": "#d9ecec", "stroke": "#0d8b8b", "ink": "#0d7c7c",
     "engage": "Tap — toggle",          "exit": "Tap again / Esc"},
    {"name": "Delete",     "tag": "Dl⊙",   "fill": "#f7ddd9", "stroke": "#c25f54", "ink": "#bb554a",
     "engage": "Hold",                  "exit": "Release (hold-only)"},
    {"name": "Tabs",       "tag": "hold",  "fill": "#ead9eb", "stroke": "#9a5fa1", "ink": "#8a4f91",
     "engage": "Hold",                  "exit": "Release (hold-only)"},
]

# Matrix rows: (key glyph, Navigation, Select, Delete, Tabs).  "·" = no special role.
ROWS = [
    ("←",  "Char left",     "Select char left",     "Backspace",            "Previous tab"),
    ("→",  "Char right",    "Select char right",    "Forward-delete",       "Next tab"),
    ("↑",  "Line up",       "Select line up",       "·",                    "New tab"),
    ("↓",  "Line down",     "Select line down",     "·",                    "Close tab"),
    ("◀◀", "Word back",     "Select word back",     "Delete word back",     "Page back"),
    ("▶▶", "Word forward",  "Select word forward",  "Delete word forward",  "Page forward"),
    ("⏮",  "Line start",    "Select to line start", "Delete to line start", "·"),
    ("⏭",  "Line end",      "Select to line end",   "Delete to line end",   "·"),
    ("PgUp","Page up",      "Select page up",       "·",                    "·"),
    ("PgDn","Page down",    "Select page down",     "·",                    "·"),
    ("●",  "·",             "·",                    "·",                    "Reopen tab"),
]

# ── layout geometry ───────────────────────────────────────────────────────────
W, MX = 980, 40
CW = W - 2 * MX                       # content width = 900
CARD_TOP, CARD_H, GAP = 98, 92, 16
CARD_W = (CW - 3 * GAP) / 4
MAT_TOP = CARD_TOP + CARD_H + 30      # matrix top
KEYW = 92                             # key column width
COLW = (CW - KEYW) / 4                # mode column width
HEADH, ROWH = 30, 27
MAT_BOTTOM = MAT_TOP + HEADH + len(ROWS) * ROWH
H = int(MAT_BOTTOM + 64)
DOTCOL = "#b9b0a6"

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
    f'letter-spacing="1.2" fill="{TITLE}">Navigation modes</text>')
add(f'<text x="{MX}" y="78" font-size="13.5" fill="{MUTED}">'
    'Hold the left inner thumb to navigate; tap or hold a trigger to layer a sub-mode on top.</text>')

# ── mode cards ────────────────────────────────────────────────────────────────
for i, m in enumerate(MODES):
    x = MX + i * (CARD_W + GAP)
    add(f'<rect x="{x+1.5:.1f}" y="{CARD_TOP+2.5}" width="{CARD_W:.1f}" height="{CARD_H}" '
        f'rx="8" ry="8" fill="#3a2f25" opacity="0.10" filter="url(#sh)"/>')
    add(f'<rect x="{x:.1f}" y="{CARD_TOP}" width="{CARD_W:.1f}" height="{CARD_H}" '
        f'rx="8" ry="8" fill="{m["fill"]}" stroke="{m["stroke"]}" stroke-width="1.3"/>')
    add(f'<text x="{x+16:.1f}" y="{CARD_TOP+28}" font-size="17" font-weight="700" '
        f'fill="{m["ink"]}">{m["name"]}</text>')
    add(f'<text x="{x+CARD_W-14:.1f}" y="{CARD_TOP+27}" font-size="12.5" font-style="italic" '
        f'text-anchor="end" fill="{m["ink"]}">{m["tag"]}</text>')
    add(f'<text x="{x+16:.1f}" y="{CARD_TOP+53}" font-size="12.5" fill="{INK}">'
        f'<tspan font-weight="600">Engage</tspan>  {m["engage"]}</text>')
    add(f'<text x="{x+16:.1f}" y="{CARD_TOP+73}" font-size="12.5" fill="{MUTED}">'
        f'<tspan font-weight="600">Exit</tspan>  {m["exit"]}</text>')

# ── matrix ────────────────────────────────────────────────────────────────────
col_x = [MX + KEYW + i * COLW for i in range(4)]          # left edge of each mode col
col_cx = [cx + COLW / 2 for cx in col_x]                  # centre of each mode col
key_cx = MX + KEYW / 2

# zebra backgrounds for the mode columns + nav-blue key column
for r in range(len(ROWS)):
    y = MAT_TOP + HEADH + r * ROWH
    if r % 2 == 1:
        add(f'<rect x="{MX+KEYW}" y="{y}" width="{CW-KEYW}" height="{ROWH}" fill="#f1ece4"/>')
add(f'<rect x="{MX}" y="{MAT_TOP+HEADH}" width="{KEYW}" height="{len(ROWS)*ROWH}" '
    f'rx="4" ry="4" fill="#dbe9f8"/>')

# header: mode names in accent colour + accent underline bar
hy = MAT_TOP + HEADH - 9
add(f'<text x="{key_cx}" y="{hy}" font-size="12" font-weight="600" text-anchor="middle" '
    f'fill="{MUTED}">Key</text>')
for i, m in enumerate(MODES):
    add(f'<text x="{col_cx[i]:.1f}" y="{hy}" font-size="13.5" font-weight="700" '
        f'text-anchor="middle" fill="{m["ink"]}">{m["name"]}</text>')
    add(f'<rect x="{col_x[i]+10:.1f}" y="{MAT_TOP+HEADH-4}" width="{COLW-20:.1f}" height="2.4" '
        f'rx="1" fill="{m["stroke"]}"/>')

# rows
for r, row in enumerate(ROWS):
    y = MAT_TOP + HEADH + r * ROWH
    ty = y + ROWH / 2 + 5
    key = row[0]
    if key in ("⏮", "⏭"):          # fallback-font glyphs render small; bump up
        ksize = 20
    elif key in ("◀◀", "▶▶"):
        ksize = 14
    elif key in ("PgUp", "PgDn"):
        ksize = 12
    else:
        ksize = 17
    # No font-weight: ⏮/⏭ exist only in fallback fonts (FreeSerif/FreeSans), and a bold
    # request breaks fontconfig's fallback (draws blank .notdef). The glyphs are solid anyway.
    add(f'<text x="{key_cx}" y="{ty:.1f}" font-size="{ksize}" '
        f'style="font-family:{MONO}" text-anchor="middle" fill="{INK}">{key}</text>')
    for i in range(4):
        val = row[i + 1]
        fill = DOTCOL if val == "·" else INK
        add(f'<text x="{col_cx[i]:.1f}" y="{ty:.1f}" font-size="12.5" text-anchor="middle" '
            f'fill="{fill}">{val}</text>')

# legend
ly = MAT_BOTTOM + 26
add(f'<text x="{MX}" y="{ly}" font-size="11.5" fill="{MUTED}">'
    '<tspan font-weight="700" fill="' + DOTCOL + '">·</tspan>  no special role in that mode — '
    'the key keeps its Navigation behaviour.   '
    'Tab actions are OS-aware (Firefox &amp; Chrome, macOS &amp; Linux).</text>')
add(f'<text x="{MX}" y="{ly+17}" font-size="11.5" fill="{MUTED}">'
    'Select and Delete are mutually exclusive — starting a deletion clears an active selection latch.</text>')

add('</svg>')
print("\n".join(s))
