#include <ncursesw/curses.h>
#include "win_utils.h"

WINDOW* window_init(WIN_INFO* info_struct) {
    return newwin(info_struct->height, info_struct->width, info_struct->starty, info_struct->startx);
};

void window_draw_border_to(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct) {
    mvwaddstr(windows[win_order_code], 0, 0, info_struct->border.tl);
    mvwaddstr(windows[win_order_code], 0, info_struct->width - 1, info_struct->border.tr);
    mvwaddstr(windows[win_order_code], info_struct->height - 1, 0, info_struct->border.bl);
    mvwaddstr(windows[win_order_code], info_struct->height - 1, info_struct->width - 1, info_struct->border.br);
    for (int i = 1; i < info_struct->width - 1; i++) { mvwaddstr(windows[win_order_code], 0, i, info_struct->border.ts); };
    for (int i = 1; i < info_struct->width - 1; i++) { mvwaddstr(windows[win_order_code], info_struct->height - 1, i, info_struct->border.bs); }
    for (int i = 1; i < info_struct->height - 1; i++) { mvwaddstr(windows[win_order_code], i, 0, info_struct->border.ls); };
    for (int i = 1; i < info_struct->height - 1; i++) { mvwaddstr(windows[win_order_code], i, info_struct->width - 1, info_struct->border.rs); };
};

void window_init_with_border(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct) {
    windows[win_order_code] = window_init(info_struct);
    window_draw_border_to(windows, win_order_code, info_struct);
};

void window_move_and_resize(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct) {
    mvwin(windows[win_order_code], info_struct->starty, info_struct->startx);
    wresize(windows[win_order_code], info_struct->height, info_struct->width);
    window_draw_border_to(windows, win_order_code, info_struct);
};