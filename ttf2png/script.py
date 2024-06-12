from fontTools.ttLib import TTFont
from PIL import Image, ImageFont, ImageDraw
import math


def ttf2png(font_path: str, font_size: float, png_path: str, gly_path: str):
    index = []
    for table in TTFont(font_path)['cmap'].tables:
        index += table.cmap.keys()
    index = list(set(index))
    index.sort()
    count = len(index)

    font = ImageFont.truetype(font_path, font_size / 3 * 4)

    tile_width = 0
    tile_height = 0
    with open(gly_path, "wb") as output:
        output.write(b'gly' + font_size.to_bytes())
        for i in index:
            left, top, right, bottom = font.getbbox(chr(i))
            tile_width = max(tile_width, right - left)
            tile_height = max(tile_height, bottom - top)
            output.write(i.to_bytes(2) + left.to_bytes(signed=True) + top.to_bytes(signed=True) + right.to_bytes(signed=True) + bottom.to_bytes(signed=True))

    image = Image.new('RGBA', (256 * tile_width, math.ceil(count / 256) * tile_height))
    draw = ImageDraw.Draw(image)
    for i, j in zip(index, range(count)):
        left, top, right, bottom = font.getbbox(chr(i))
        x = j % 256 * tile_width - left
        y = j // 256 * tile_height - top
        draw.text((x, y), chr(i), font=font, fill=(255, 255, 255))
    image.save(png_path)


ttf2png('./font.ttf', 12, './font.png', './font.gly')
