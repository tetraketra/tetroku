#include <time.h>
#include <unistd.h>
#include <ncursesw/panel.h>

#include "ncurses_setup.h"
#include "win_info.h"
#include "win_utils.h"

#define MAXLINES (LINES - 1)
#define MAXCOLS (COLS - 1)

#define SPECIAL_EXIT_KEY 'q'

#define CLEAR_WINDOWS(windows, num_windows)     \
    for (int i = 0; i < num_windows; i++) {     \
        werase(windows[i]);                     \
    }
#define DISPLAY_WINDOWS(windows, panels, num_windows)   \
    for (int i = 0; i < _WINDOWS; i++) {                \
        panels[i] = new_panel(windows[i]);              \
    }                                                   \
    update_panels();                                    \
    doupdate();

enum WINDOW_ORDER {
    WO_TL,
    WO_TM,
    WO_TR,
    WO_ML,
    WO_MM,
    WO_MR,
    WO_BL,
    WO_BM,
    WO_BR,
    _WINDOWS,
};

int main(void) {
    const int FPS = 60;
    const int FRAME_TIME = 1000000 / FPS;
    clock_t start_time;

    WINDOW* windows[_WINDOWS];
    PANEL* panels[_WINDOWS];

    ncurses_setup(FPS);

    char ch;
    while (true) {
        ch = getch();
        start_time = clock();

        CLEAR_WINDOWS(windows, _WINDOWS);

        switch (ch) {
            case SPECIAL_EXIT_KEY:
                endwin();
                return 0;
                break;
        }

        DISPLAY_WINDOWS(windows, panels, _WINDOWS)

        long sleep_time = FRAME_TIME - (clock() - start_time);
        if (sleep_time > 0)
            usleep(sleep_time);

    }

    

    return 0;
}