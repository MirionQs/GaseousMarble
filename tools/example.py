from font_generator import font_generator

char_list = '''
To be or not to be, that is the question
生存还是毁灭，这是一个值得考虑的问题
生存還是毀滅，這是一個值得考慮的問題
'''

generator = font_generator(['./fonts/font_default.ttf', './fonts/font_alternate.ttf'], 16, char_list, stroke_width=1)

generator.generate('../example.gm82/plugins/font_default.png', '../example.gm82/plugins/font_default.gly')
generator.generate('../test.gm82/plugins/font_default.png', '../test.gm82/plugins/font_default.gly')
