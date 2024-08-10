from fontTools.ttLib import TTFont
from PIL import Image, ImageFont, ImageDraw
import math
import struct


def generate_font(font_path: str | list[str], font_size: int, png_path: str, gly_path: str,
                  char_list: str | None = None, anti_aliasing: bool = True, glyph_spacing: int = 0, stroke_width: int = 0):

    if isinstance(font_path, str):
        font_path = [font_path]

    cp_dict = {}
    cp_list = set()
    for path in font_path:
        cp = set()
        for table in TTFont(path)['cmap'].tables:
            cp |= table.cmap.keys()
        cp_dict[path] = set(filter(lambda x: x <= 65535, cp - cp_list))
        cp_list |= cp_dict[path]
    if char_list != None:
        char_list_cp = set(map(ord, char_list))
        for cp in cp_dict.values():
            cp &= char_list_cp
    cp_dict = {path: sorted(cp) for (path, cp) in cp_dict.items() if len(cp) != 0}

    font_list = [ImageFont.truetype(path, font_size) for path in cp_dict.keys()]
    total_width = 0
    line_height = 0
    for (font, cp) in zip(font_list, cp_dict.values()):
        for ch in cp:
            (left, _, right, bottom) = font.getbbox(chr(ch))
            width = right - left + stroke_width * 2 + glyph_spacing
            height = bottom + stroke_width * 2 + glyph_spacing
            total_width += width
            line_height = max(line_height, height)

    if total_width <= 1023:
        max_width = total_width
        image = Image.new('RGBA', (max_width, line_height))
    else:
        max_width = math.ceil((line_height + math.sqrt(line_height * (line_height + 4 * total_width))) / 2)  # x == h * (w / x + 1)
        line_width = 0
        line_count = 1
        for (font, cp) in zip(font_list, cp_dict.values()):
            for ch in cp:
                (left, _, right, _) = font.getbbox(chr(ch))
                width = right - left + stroke_width * 2 + glyph_spacing
                if line_width + width > max_width:
                    line_width = 0
                    line_count += 1
                line_width += width
        image = Image.new('RGBA', (max_width, line_height * line_count))

    draw = ImageDraw.Draw(image)
    if not anti_aliasing:
        draw.fontmode = '1'
    with open(gly_path, "wb") as gly:
        gly.write(b'GLY\0' + struct.pack("2B", font_size + stroke_width * 2, line_height))
        x = 0
        y = 0
        for (font, cp) in zip(font_list, cp_dict.values()):
            draw.font = font
            for ch in cp:
                (left, _, right, _) = font.getbbox(chr(ch))
                width = right - left + stroke_width * 2 + glyph_spacing
                if x + width >= max_width:
                    x = 0
                    y += line_height
                draw.text((x - left + stroke_width, y + stroke_width), chr(ch), 'white', stroke_width=stroke_width, stroke_fill='black')
                gly.write(struct.pack("3HBb", ch, x, y, width, left - stroke_width))
                x += width
    image.save(png_path)


generate_font(
    ['./example.gm82/plugins/font_default.ttf', './example.gm82/plugins/font_supplemental.ttf'],
    16,
    './example.gm82/plugins/font_default.png',
    './example.gm82/plugins/font_default.gly',
    char_list='To be or not to be, that is the question\n生存还是毁灭，这是一个值得考虑的问题\n生存還是毀滅，這是一個值得考慮的問題',
    anti_aliasing=False,
    stroke_width=1
)
