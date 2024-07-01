#define Create_0
/*"/*'/**//* YYD ACTION
lib_id=1
action_id=603
applies_to=self
*/
var path{path='../Debug/GaseousMarble.dll'}

global.gm_init=external_define(path,'gm_init',dll_cdecl,ty_real,1,ty_real)
global.gm_font=external_define(path,'gm_font',dll_cdecl,ty_real,2,ty_string,ty_string)
global.gm_draw=external_define(path,'gm_draw',dll_cdecl,ty_real,3,ty_real,ty_real,ty_string)
global.gm_set_color=external_define(path,'gm_set_color',dll_cdecl,ty_real,2,ty_real,ty_real)
global.gm_set_align=external_define(path,'gm_set_align',dll_cdecl,ty_real,2,ty_real,ty_real)

external_call(global.gm_init,get_function_address('get_function_address'))
font=external_call(global.gm_font,'../ttf2png/font.png','../ttf2png/font.gly')
#define Draw_0
/*"/*'/**//* YYD ACTION
lib_id=1
action_id=603
applies_to=self
*/
external_call(global.gm_set_color,c_yellow,c_orange)
external_call(global.gm_set_align,0,0)
external_call(global.gm_draw,160,120,'Lorem Ipsum
乱数假文
一去二三里，烟村四五家')
