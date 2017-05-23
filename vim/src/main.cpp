#include <stdlib.h>
#include <iostream>
#include "display.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>", argv[0]);
        exit(1);
    }

    /* Make and run the game */
    Display d;

    /* Create windows */
    WindowSPtr w1 = d.createWindow(0, 0, COLS, LINES-1);
    WindowSPtr w2 = d.createWindow(0, LINES-1, COLS, LINES);

    for (int i = 1; i < LINES; i++)
        for (int j = 0; j < i; j++)
            d.putChar(w1, j, i-1, '.');

    int x = 0;
    while (1) {
        /* Temporary just to show functionality. */
        int c = getch();

        if (c == KEY_LEFT || c == 27 /* ESCAPE */ || c == 10 /* ENTER */)
            break;
        else {
            d.putChar(w2, (x / 80), (x % 80), c); /* BACKSPACE = 127 */
        }
        x++;
    }

    return 0;
}
