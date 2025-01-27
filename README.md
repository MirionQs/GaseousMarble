# GaseousMarble

Draw UCS-2 characters in GameMaker 8.2.

## Functions

GaseousMarble provides following functions for drawing texts

| **Function** | **Description** | **Comment** |
| -- | -- | -- |
| `gm_font(sprite_path, glyph_path)` | Adds a font and returns its id. Returns `0` if failed. | The font sprite and glyph data can be generated using `tools/font_generator.py`. |
| `gm_width(text)` | Returns the width of the text. | |
| `gm_height(text)` | Returns the height of the text. | |
| `gm_free(font_id)` | Frees a specified font. Returns `false` if failed. | |
| `gm_clear()` | Frees all loaded fonts. | Use this function before calling `game_restart` to prevent memory leaks. |
| `gm_draw(x, y, text)` | Draws a UTF-8 encoded string. Returns `false` if failed. | |

as well as setters and getters for configuring the drawing parameters

| **Setter** | **Getter** |
| -- | -- |
| `gm_set_font(font_id)` | `gm_get_font()` |
| `gm_set_color(color)`<br>`gm_set_color2(color_top, color_bottom)` | `gm_get_color_top()`<br>`gm_get_color_bottom()` |
| `gm_set_alpha(alpha)` | `gm_get_alpha()` |
| `gm_set_halign(align)`<br>`gm_set_valign(align)`<br>`gm_set_align(halign, valign)` | `gm_get_halign()`<br>`gm_get_valign()` |
| `gm_set_max_line_width(max_width)` | `gm_get_max_line_width()` |
| `gm_set_letter_spacing(spacing)` | `gm_get_letter_spacing()` |
| `gm_set_word_spacing(spacing)` | `gm_get_word_spacing()` |
| `gm_set_line_height(height)` | `gm_get_line_height()` |
| `gm_set_offset(x, y)`| `gm_get_offset_x()`<br>`gm_get_offset_y()` |
| `gm_set_scale(x, y)` | `gm_get_scale_x()`<br>`gm_get_scale_y()` |

If the operation fails (e.g., due to invalid arguments), the corresponding function will return `false`.

## Comment

The current drawing principle of GaseousMarble is using a python script to generate sprite images and glyph data, and then calling GameMaker built-in functions to draw part of the sprites. But this is not flexible, so I plan to extract textures from the font file, put them on a specific sprite, and draw the sprite.

Open the project with Visual Studio 2022. The files under the filter `api` are the underlying interfaces for interacting with GameMaker. The files under the filter `gm` (which stands for GaseousMarble) implement the drawing function, and `utils.h` implements the encoding conversion.