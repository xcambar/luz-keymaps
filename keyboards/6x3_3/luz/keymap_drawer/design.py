#!/usr/bin/env python3
"""SVG post-processing for the Luz keymap-drawer diagrams, shared by build.sh and build_pdf.sh.

    design.py nudge FILE   bake the CSS corner-glyph transform into SVG attributes
    design.py style FILE   apply the "Direction A" look (color SVGs only)

Both rewrite FILE in place. The YAML CSS stays the palette/semantics source; these only
layer on what keymap-drawer cannot express.
"""
import re
import sys


def nudge(s):
    # ImageMagick's SVG coder honors the SVG transform *attribute* but ignores the CSS
    # transform *property*. keymap-drawer only emits CSS, so bake the corner-glyph nudge
    # (.key.delhint.bl / .key.selhint.br) from the CSS into transform attributes on the
    # bl/br elements. Keeps the YAML CSS as the single source of truth. (rsvg-convert is
    # not an option: it drops keymap-drawer's <use>-of-nested-<svg> glyphs entirely.)
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
    return re.sub(r'<(?:text|use)\b[^>]*?class="([^"]*)"[^>]*?>', repl, s)


def style(s):
    # "Direction A" visual system. Humanist sans for labels, mono kept for literal single-
    # glyph keycodes ("this is what you type"); warm paper ground; refined caps floating on a
    # soft shadow; warm-neutral inactive keys; editorial layer title. Rendered with Inkscape —
    # the only backend that does the font + shadow filter + keymap-drawer's <use> glyphs at once.
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
    return s


if __name__ == '__main__':
    op, path = sys.argv[1], sys.argv[2]
    fn = {'nudge': nudge, 'style': style}[op]
    with open(path, encoding='utf-8') as f:
        s = f.read()
    with open(path, 'w', encoding='utf-8') as f:
        f.write(fn(s))
