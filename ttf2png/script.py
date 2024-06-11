from fontTools.ttLib import TTFont
from PIL import Image, ImageFont, ImageDraw
import math


def ttf2png(fontpath, fontsize, pngpath, glypath):
    index = []
    for table in TTFont(fontpath)['cmap'].tables:
        index += table.cmap.keys()
    index = list(set(index))
    index.sort()

    font = ImageFont.truetype(fontpath, fontsize / 3 * 4)

    width = 0
    height = 0
    with open(glypath, "wb") as output:
        for i in index:
            left, top, right, bottom = font.getbbox(chr(i))
            width = max(width, right - left)
            height = max(height, bottom - top)
            output.write(
                i.to_bytes(2) +
                left.to_bytes(signed=True) +
                top.to_bytes(signed=True) +
                right.to_bytes(signed=True) +
                bottom.to_bytes(signed=True)
            )

    image = Image.new('RGBA', (256 * width, math.ceil(len(index) / 256) * height))
    draw = ImageDraw.Draw(image)
    for i, j in zip(index, range(len(index))):
        x, y = j % 256 * width, j // 256 * height
        left, top, right, bottom = font.getbbox(chr(i))
        draw.text((x - left, y - top), chr(i), font=font, fill=(255, 255, 255))
    image.save(pngpath)


ttf2png('./font.ttf', 13, './font.png', './font.gly')
