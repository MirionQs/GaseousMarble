#define Create_0
/*"/*'/**//* YYD ACTION
lib_id=1
action_id=603
applies_to=self
*/
var dllpath{ dllpath = '../Debug/GaseousMarble.dll' }

global.gm_init = external_define(dllpath, 'gm_init', dll_cdecl, ty_real, 1, ty_real)
global.gm_font = external_define(dllpath, 'gm_font', dll_cdecl, ty_real, 2, ty_string, ty_string)
global.gm_draw = external_define(dllpath, 'gm_draw', dll_cdecl, ty_real, 3, ty_real, ty_real, ty_string)
global.gm_free = external_define(dllpath, 'gm_free', dll_cdecl, ty_real, 1, ty_real)
global.gm_set_color2 = external_define(dllpath, 'gm_set_color2', dll_cdecl, ty_real, 2, ty_real, ty_real)
global.gm_set_align = external_define(dllpath, 'gm_set_align', dll_cdecl, ty_real, 2, ty_real, ty_real)

external_call(global.gm_init, get_function_address('get_function_address'))
global.font_default = external_call(global.gm_font, './plugins/font_default.png', './plugins/font_default.gly')
#define Keyboard_82
/*"/*'/**//* YYD ACTION
lib_id=1
action_id=603
applies_to=self
*/
external_call(global.gm_free, global.font_default)

game_restart()
#define Draw_0
/*"/*'/**//* YYD ACTION
lib_id=1
action_id=603
applies_to=self
*/
external_call(global.gm_set_color2, c_yellow, c_orange)
external_call(global.gm_set_align, 0, 0)
external_call(global.gm_draw, 160, 120, 'Lorem Ipsum
乱数假文
一去二三里，烟村四五家')
