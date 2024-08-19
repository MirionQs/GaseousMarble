from fontTools.ttLib import TTFont
from PIL import Image, ImageFont, ImageDraw
import math
import struct


class font_generator:
    def __init__(self, font_path: str | list[str], font_size=16, char_list: str | None = None, *,
                 smoothing=True, outlined=False, stroke_width=0):
        if isinstance(font_path, str):
            font_path = [font_path]
        if isinstance(char_list, str):
            char_list = set(filter(lambda x: x >= 32, map(ord, char_list)))

        if outlined and stroke_width == 0:
            raise Exception('The stroke width of outlined fonts must be greater than 0.')

        self.font_path = font_path
        self.font_size = font_size
        self.char_list = char_list
        self.outlined = outlined
        self.anti_aliasing = smoothing
        self.stroke_width = stroke_width
        self.code_point = {}

        if char_list != None:
            self.update()

    def update(self):
        cp_dict = {}
        cp_list = set()
        char_list = self.char_list

        for path in self.font_path:
            cp = set()
            with TTFont(path) as font:
                for table in font['cmap'].tables:
                    cp |= table.cmap.keys()
            cp -= cp_list

            cp_dict[path] = set(filter(lambda x: 32 <= x < 65536, cp))
            cp_list |= cp_dict[path]

        if char_list != None:
            for cp in cp_dict.values():
                cp &= char_list
                char_list -= cp

            if len(char_list) != 0:
                raise Exception('Unable to find a suitable font for following characters: ' + ''.join(map(chr, char_list)))

        self.code_point = {path: sorted(cp) for (path, cp) in cp_dict.items()}

    def generate(self, sprite_path: str, glyph_path: str):
        font_list = [ImageFont.truetype(path, self.font_size) for path in self.code_point]

        total_width = 0
        line_height = 0
        for (font, cp) in zip(font_list, self.code_point.values()):
            for i in cp:
                (left, _, right, bottom) = font.getbbox(chr(i))
                width = right - left + self.stroke_width * 2
                height = bottom + self.stroke_width * 2

                total_width += width
                line_height = max(line_height, height)

        if total_width < 1024:
            max_width = total_width
            image = Image.new('RGBA', (max_width, line_height))
        else:
            max_width = math.ceil((line_height + math.sqrt(line_height * (line_height + 4 * total_width))) / 2)  # x == h * (w / x + 1)

            line_width = 0
            line_count = 1
            for (font, cp) in zip(font_list, self.code_point.values()):
                for i in cp:
                    (left, _, right, _) = font.getbbox(chr(i))
                    width = right - left + self.stroke_width * 2

                    if line_width + width > max_width:
                        line_width = 0
                        line_count += 1
                    line_width += width

            image = Image.new('RGBA', (max_width, line_height * line_count))

        draw = ImageDraw.Draw(image)
        if not self.anti_aliasing:
            draw.fontmode = '1'

        with open(glyph_path, "wb") as file:
            file.write(b'GLY\0' + struct.pack("2H", self.font_size + self.stroke_width * 2, line_height))

            x = 0
            y = 0
            for (font, cp) in zip(font_list, self.code_point.values()):
                draw.font = font
                for i in cp:
                    (left, _, right, _) = font.getbbox(chr(i))
                    width = right - left + self.stroke_width * 2
                    if x + width > max_width:
                        x = 0
                        y += line_height
                    file.write(struct.pack("4Hh", i, x, y, width, left - self.stroke_width))

                    pos = (x - left + self.stroke_width, y + self.stroke_width)
                    if self.outlined:
                        draw.text(pos, chr(i), '#0000', stroke_width=self.stroke_width, stroke_fill='white')
                    else:
                        draw.text(pos, chr(i), 'white', stroke_width=self.stroke_width, stroke_fill='black')

                    x += width
        image.save(sprite_path)


font_dir = '../test.gm82/plugins/'

char_list = '''
To be or not to be, that is the question
生存还是毁灭，这是一个值得考虑的问题
生存還是毀滅，這是一個值得考慮的問題
'''

generator = font_generator([font_dir + 'font_default.ttf', font_dir + 'font_alternate.ttf'], 16, char_list, stroke_width=1)

generator.generate(font_dir + 'font_default.png', font_dir + 'font_default.gly')
