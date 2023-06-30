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
    };
#define DISPLAY_WINDOWS(windows, panels, num_windows)   \
    for (int i = 0; i < num_windows; i++) {             \
        panels[i] = new_panel(windows[i]);              \
    };                                                  \
    update_panels();                                    \
    doupdate();
#define DRAW_WINDOW_BORDERS(windows, info_structs, num_windows) \
    for (int i = 0; i < num_windows; i++) {                     \
        window_draw_border_to(windows, i, &info_structs[i]);    \
    };

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

    ncurses_setup(FPS);

    WIN_INFO info_structs[_WINDOWS];
    WINDOW* windows[_WINDOWS];
    PANEL* panels[_WINDOWS];

    int board_start_y = 0;
    int board_start_x = 0;
    int box_width = 9;
    int box_height = 5;
    
    WIN_BORDER win_borders[_WINDOWS] = {
        WB_CONNECTS_RIGHT_DOWN,    WB_CONNECTS_LR_DOWN,    WB_CONNECTS_LEFT_DOWN,
        WB_CONNECTS_RIGHT_UP_DOWN, WB_CONNECTS_LR_UP_DOWN, WB_CONNECTS_LEFT_UP_DOWN,
        WB_CONNECTS_RIGHT_UP,      WB_CONNECTS_LR_UP,      WB_CONNECTS_LEFT_UP,
    };
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            info_structs[3*row + col] = (WIN_INFO) {
                .starty = board_start_x + row*(box_height - 1), 
                .startx = board_start_y + col*(box_width - 1),
                .height = box_height, .width = box_width,
                .border = win_borders[3*row + col]
            };

            window_init_with_border(windows, 3*row + col, &info_structs[3*row + col]);
        }
    }
    DISPLAY_WINDOWS(windows, panels, _WINDOWS);

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

            default:
                break;
        }

        DRAW_WINDOW_BORDERS(windows, info_structs, _WINDOWS);        
        DISPLAY_WINDOWS(windows, panels, _WINDOWS);

        long sleep_time = FRAME_TIME - (clock() - start_time);
        if (sleep_time > 0)
            usleep(sleep_time);

    }

    

    return 0;
}