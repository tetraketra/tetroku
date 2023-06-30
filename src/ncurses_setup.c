#include <stdlib.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include "ncurses_setup.h"

inline void ncurses_setup(int FPS) {
    setlocale(LC_ALL, "");
    initscr();

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color!\n");
        exit(1);
    }
    start_color();
    
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    halfdelay(1/FPS);
    cbreak();
    noecho();

    clear();
    refresh();
}