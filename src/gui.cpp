//
// Created by bertoc2 on 1/14/2023.
//

#include "gui.h"

void init_gui() {
    initscr();			/* Start curses mode 		  */

    raw();
    keypad(stdscr, 1);
    noecho();

    start_color();

    use_default_colors();

    for (int i = 0; i < 1000; i++) {
        init_pair(i + 1, -1, i);
    }

    for (int i = 0; i < 1000; i++) {
        attron(COLOR_PAIR(i));
        addstr(" ");
    }

    char ch = getch();
    addch(ch | A_BOLD | A_UNDERLINE | 0);

    refresh();			/* Print it on to the real screen */
    getch();			/* Wait for user input */
    endwin();			/* End curses mode		  */
}