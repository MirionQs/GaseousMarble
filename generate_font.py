from fontTools.ttLib import TTFont
from PIL import Image, ImageFont, ImageDraw
import math
import struct


def get_code_point(font_path: str) -> set[int]:
    ttfont = TTFont(font_path)
    code_point_list = set()
    for table in ttfont['cmap'].tables:
        code_point_list |= table.cmap.keys()
    return code_point_list


def generate_font(font_path: str, font_size: int, png_path: str, gly_path: str,
                  char_list: str | None = None, glyph_spacing: int = 0, stroke_width: int = 0):
    code_point_list = get_code_point(font_path)
    if char_list != None:
        code_point_list &= set(map(ord, char_list))
    code_point_list = list(filter(lambda ch: ch <= 65535, code_point_list))
    code_point_list.sort()

    font = ImageFont.truetype(font_path, font_size)
    total_width = 0
    line_height = 0
    for ch in code_point_list:
        (left, _, right, bottom) = font.getbbox(chr(ch))
        width = right - left + stroke_width * 2 + glyph_spacing
        height = bottom + stroke_width * 2 + glyph_spacing
        total_width += width
        line_height = max(line_height, height)

    if total_width <= 1023:
        max_width = total_width
        image = Image.new('RGBA', (max_width, 1))
    else:
        max_width = math.ceil((line_height + math.sqrt(line_height * (line_height + 4 * total_width))) / 2)  # x == h * (w / x + 1)
        line_width = 0
        line_count = 1
        for ch in code_point_list:
            (left, _, right, _) = font.getbbox(chr(ch))
            width = right - left + stroke_width * 2 + glyph_spacing
            if line_width + width > max_width:
                line_width = 0
                line_count += 1
            line_width += width
        image = Image.new('RGBA', (max_width, line_height * line_count))

    draw = ImageDraw.Draw(image)
    draw.font = font

    with open(gly_path, "wb") as gly:
        gly.write(b'GLY\0' + struct.pack("2B", font_size + stroke_width * 2, line_height))
        x = 0
        y = 0
        for ch in code_point_list:
            (left, _, right, _) = font.getbbox(chr(ch))
            width = right - left + stroke_width * 2 + glyph_spacing
            if x + width >= max_width:
                x = 0
                y += line_height
            draw.text((x - left + stroke_width, y + stroke_width), chr(ch), 'white', stroke_width=stroke_width, stroke_fill='black')
            gly.write(struct.pack("3HBb", ch, x, y, width, left - stroke_width))
            x += width
    image.save(png_path)


generate_font('./example.gm82/plugins/font_default.ttf', 16, './example.gm82/plugins/font_default.png', './example.gm82/plugins/font_default.gly', stroke_width=1)
