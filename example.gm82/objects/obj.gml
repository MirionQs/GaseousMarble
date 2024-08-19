#define Create_0
/*"/*'/**//* YYD ACTION
lib_id=1
action_id=603
applies_to=self
*/
global.font_default = gm_font('../test.gm82/plugins/font_default.png', '../test.gm82/plugins/font_default.gly')
gm_set_font(global.font_default)
#define Keyboard_82
/*"/*'/**//* YYD ACTION
lib_id=1
action_id=603
applies_to=self
*/
gm_clear()
game_restart()
#define Draw_0
/*"/*'/**//* YYD ACTION
lib_id=1
action_id=603
applies_to=self
*/
gm_set_color2(c_yellow, c_orange)
gm_set_align(0, 0)
gm_draw(room_width / 2, room_height / 2, 'To be or not to be, that is the question
生存还是毁灭，这是一个值得考虑的问题
生存還是毀滅，這是一個值得考慮的問題')
