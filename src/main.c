#include <time.h>
#include <unistd.h>
#include <ncursesw/curses.h>
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
    // setup
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
    int total_board_width = box_width*3 - 3;
    int total_board_height = box_height*3 - 3;
    
    // sudoku boxes
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

    // draw loop
    int active_window = WO_MM;
    int window_curs_y = 1;
    int window_curs_x = 2;

    int ch;
    while (true) {
        wmove(windows[active_window], window_curs_y, window_curs_x);
        wrefresh(windows[active_window]);
        
        ch = getch();
        start_time = clock();

        CLEAR_WINDOWS(windows, _WINDOWS);

        switch (ch) {
            case SPECIAL_EXIT_KEY:
                endwin();
                return 0;
                break;

            case KEY_DOWN ... KEY_RIGHT:

                // on game board
                if (active_window <= WO_BR) {
                    switch (ch) {
                        case KEY_DOWN:
                            if (window_curs_y == 3) {
                                switch (active_window) {
                                    case WO_TL: active_window = WO_ML; window_curs_y = 1; break;
                                    case WO_ML: active_window = WO_BL; window_curs_y = 1; break;
                                    case WO_BL: break;
                                    case WO_TM: active_window = WO_MM; window_curs_y = 1; break;
                                    case WO_MM: active_window = WO_BM; window_curs_y = 1; break;
                                    case WO_BM: break;
                                    case WO_TR: active_window = WO_MR; window_curs_y = 1; break;
                                    case WO_MR: active_window = WO_BR; window_curs_y = 1; break;
                                    case WO_BR: break;
                                }
                            } else {
                                window_curs_y += 1;
                            } 
                            break;

                        case KEY_UP:
                            if (window_curs_y == 1) {
                                switch (active_window) {
                                    case WO_TL: break;
                                    case WO_ML: active_window = WO_TL; window_curs_y = 3; break;
                                    case WO_BL: active_window = WO_ML; window_curs_y = 3; break;
                                    case WO_TM: break;
                                    case WO_MM: active_window = WO_TM; window_curs_y = 3; break;
                                    case WO_BM: active_window = WO_MM; window_curs_y = 3; break;
                                    case WO_TR: break;
                                    case WO_MR: active_window = WO_TR; window_curs_y = 3; break;
                                    case WO_BR: active_window = WO_MR; window_curs_y = 3; break;
                                }
                            } else {
                                window_curs_y -= 1;
                            }
                            break;

                        case KEY_LEFT:
                            if (window_curs_x == 2) {
                                switch (active_window) {
                                    case WO_TL: break;
                                    case WO_ML: break;
                                    case WO_BL: break;
                                    case WO_TM: active_window = WO_TL; window_curs_x = 6; break;
                                    case WO_MM: active_window = WO_ML; window_curs_x = 6; break;
                                    case WO_BM: active_window = WO_BL; window_curs_x = 6; break;
                                    case WO_TR: active_window = WO_TM; window_curs_x = 6; break;
                                    case WO_MR: active_window = WO_MM; window_curs_x = 6; break;
                                    case WO_BR: active_window = WO_BM; window_curs_x = 6; break;
                                }
                            } else {
                                window_curs_x -= 2;
                            }
                            break;

                        case KEY_RIGHT:
                            if (window_curs_x == 6) {
                                switch (active_window) {
                                    case WO_TL: active_window = WO_TM; window_curs_x = 2; break;
                                    case WO_ML: active_window = WO_MM; window_curs_x = 2; break;
                                    case WO_BL: active_window = WO_BM; window_curs_x = 2; break;
                                    case WO_TM: active_window = WO_TR; window_curs_x = 2; break;
                                    case WO_MM: active_window = WO_MR; window_curs_x = 2; break;
                                    case WO_BM: active_window = WO_BR; window_curs_x = 2; break;
                                    case WO_TR: break;
                                    case WO_MR: break;
                                    case WO_BR: break;
                                }
                            } else {
                                window_curs_x += 2;
                            }
                            break;
                    }
                }

                // TODO other window movements
                break;


            case KEY_RESIZE: 
                // no special handling right now  
                break;
        }

        DRAW_WINDOW_BORDERS(windows, info_structs, _WINDOWS);
        DISPLAY_WINDOWS(windows, panels, _WINDOWS);
        // other draw calls

        long sleep_time = FRAME_TIME - (clock() - start_time);
        if (sleep_time > 0)
            usleep(sleep_time);

    }

    

    return 0;
}