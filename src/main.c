#define MAXLINES (LINES - 1)
#define MAXCOLS (COLS - 1)

#define SPECIAL_EXIT_KEY 'q'

#include "ncurses_setup.h"

#include <ncursesw/curses.h>
#include <time.h>
#include <unistd.h>

int main(void) {
    const int FPS = 60;
    const int FRAME_TIME = 1000000 / FPS;
    clock_t start_time;

    // WINDOW* windows[_WINDOWS];

    ncurses_setup(FPS);

    char ch;
    while (true) {
        ch = getch();
        start_time = clock();
        
        switch (ch) {
            case SPECIAL_EXIT_KEY:
                endwin();
                return 0;
                break;
        }

        long sleep_time = FRAME_TIME - (clock() - start_time);
        if (sleep_time > 0)
            usleep(sleep_time);

    }

    

    return 0;
}