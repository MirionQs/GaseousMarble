from fontTools.ttLib import TTFont
from PIL import Image, ImageFont, ImageDraw


def ttf2png(font_path: str, font_size_pt: int, png_path: str, gly_path: str, char_list: str | None = None, max_width: int = 0x3fff):
    ttfont = TTFont(font_path)
    code_list = set()
    for table in ttfont['cmap'].tables:
        code_list |= table.cmap.keys()
    if char_list != None:
        code_list &= map(ord, char_list)
    code_list = list(filter(lambda ch: ch <= 0xffff, code_list))
    code_list.sort()

    font = ImageFont.truetype(font_path, font_size_pt * 4 / 3)
    line_width = 0
    line_count = 1
    line_height = 0
    for c in code_list:
        left, top, right, bottom = font.getbbox(chr(c))
        width = right - left
        if line_width + width >= max_width:
            line_width = 0
            line_count += 1
        line_width += width + 1
        line_height = max(line_height, bottom)

    image = Image.new('RGBA', (max_width, line_height * line_count))
    draw = ImageDraw.Draw(image)
    draw.font = font
    with open(gly_path, "wb") as gly:
        gly.write(b'GLY\0' + font_size_pt.to_bytes() + line_height.to_bytes())
        x = 0
        y = 0
        for c in code_list:
            left, top, right, bottom = font.getbbox(chr(c))
            width = right - left
            if x + width >= max_width:
                x = 0
                y += line_height
            draw.text((x - left, y), chr(c), 'white')
            gly.write(c.to_bytes(2) + x.to_bytes(2) + y.to_bytes(2) + width.to_bytes() + left.to_bytes(signed=True))
            x += width + 1
    image.save(png_path)


ttf2png('./font.ttf', 12, './font.png', './font.gly')
