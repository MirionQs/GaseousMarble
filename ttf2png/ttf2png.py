from fontTools.ttLib import TTFont
from PIL import Image, ImageFont, ImageDraw


def ttf2png(font_path: str, font_size: int, png_path: str, gly_path: str,
            char_list: str = '', max_width: int = 0x4000, glyph_spacing: int = 1, stroke_width: int = 0):

    ttfont = TTFont(font_path)
    code_list = set()
    for table in ttfont['cmap'].tables:
        code_list |= table.cmap.keys()
    if char_list != '':
        code_list &= set(map(ord, char_list))
    code_list = list(filter(lambda ch: ch <= 0xffff, code_list))
    code_list.sort()

    font = ImageFont.truetype(font_path, font_size)
    line_width = 0
    line_count = 1
    line_height = 0
    for c in code_list:
        left, top, right, bottom = font.getbbox(chr(c))
        width = right - left + stroke_width * 2
        height = bottom + stroke_width * 2
        if line_width + width >= max_width:
            line_width = 0
            line_count += 1
        line_width += width + glyph_spacing
        line_height = max(line_height, height)

    if line_count == 1:
        image = Image.new('RGBA', (line_width, line_height))
    else:
        image = Image.new('RGBA', (max_width, line_height * line_count))
    draw = ImageDraw.Draw(image)
    draw.font = font

    with open(gly_path, "wb") as gly:
        gly.write(b'GLY\0' + (font_size + stroke_width * 2).to_bytes() + line_height.to_bytes())
        x = 0
        y = 0
        for c in code_list:
            left, top, right, bottom = font.getbbox(chr(c))
            width = right - left + stroke_width * 2
            if x + width >= max_width:
                x = 0
                y += line_height
            draw.text((x - left + stroke_width, y + stroke_width), chr(c), 'white', stroke_width=stroke_width, stroke_fill='black')
            gly.write(c.to_bytes(2) + x.to_bytes(2) + y.to_bytes(2) + width.to_bytes() + (left - stroke_width).to_bytes(signed=True))
            x += width + glyph_spacing
    image.save(png_path)


ttf2png('./font.ttf', int(12 * 4 / 3), './font.png', './font.gly', stroke_width=1)
