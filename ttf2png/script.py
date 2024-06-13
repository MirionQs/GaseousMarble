from fontTools.ttLib import TTFont
from PIL import Image, ImageFont, ImageDraw


def ttf2png(font_path: str, font_size_pt: int, png_path: str, gly_path: str, char_list: str | None = None, max_width: int = 0x3fff):
    ttfont = TTFont(font_path)
    char_code = []
    for table in ttfont['cmap'].tables:
        char_code += table.cmap.keys()
    if char_list != None:
        char_code += list(map(ord, char_list))
    char_code = list(set(char_code))
    char_code.sort()

    font = ImageFont.truetype(font_path, font_size_pt * 4 / 3)
    line_width = 0
    line_count = 1
    line_height = 0
    for ch in char_code:
        left, top, right, bottom = font.getbbox(chr(ch))
        width = right - left
        line_width += width
        if line_width >= max_width:
            line_width = width
            line_count += 1
        line_height = max(line_height, bottom)

    image = Image.new('RGBA', (max_width, line_height * line_count))
    draw = ImageDraw.Draw(image)
    draw.font = font
    with open(gly_path, "wb") as gly:
        gly.write(b'GLY\0' + font_size_pt.to_bytes() + line_height.to_bytes())
        x = 0
        y = 0
        for ch in char_code:
            left, top, right, bottom = font.getbbox(chr(ch))
            width = right - left
            if x + width >= max_width:
                x = 0
                y += line_height
            draw.text((x - left, y), chr(ch), 'white')
            gly.write(ch.to_bytes(2) + x.to_bytes(2) + y.to_bytes(2) + width.to_bytes() + left.to_bytes(signed=True))
            x += width
    image.save(png_path)


ttf2png('./font.ttf', 12, './font.png', './font.gly')
