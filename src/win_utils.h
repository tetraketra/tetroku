#ifndef NCURSES_WINDOW_UTILS_H
#define NCURSES_WINDOW_UTILS_H

#include <ncursesw/curses.h>
#include "win_info.h"

extern WINDOW* window_init(WIN_INFO* info_struct);
extern void window_draw_border_to(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);
extern void window_init_with_border(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);
extern void window_move_and_resize(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);

#endif