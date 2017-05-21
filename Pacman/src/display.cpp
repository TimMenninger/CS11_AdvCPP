#include "display.hpp"
#include <iostream>

/*
 Display

 Constructor.  This initializes the screen and windows.

 Arguments: Map map - The map contained in this display.

 Returns:   None.
*/
Display::Display(std::shared_ptr<Map> map) {
    /* Initialize the screen */
    initscr();

    /* Set options */
    cbreak();
    noecho();

    /* Use keyboard */
    raw();
    keypad(stdscr, TRUE);

    /* No cursor */
    curs_set(0);
    /* Create windows */
    _mapWindow = WindowSPtr(newwin(map->getHeight(), map->getWidth(),
                         LINES - map->getHeight(), 0));
    _textWindow = WindowSPtr(newwin(LINES - map->getHeight(), map->getWidth(),
                         0, 0));

    /* Refresh the entire thing to apply changes. */
    refresh();

    /* Draw the map */
    drawMap(map);
}

/*
 ~Display

 Destructor.  This cleans up the screen and window.

 Arguments: None.

 Returns:   None.
*/
Display::~Display() {
    /* End the session */
    endwin();
    delwin(_textWindow);
}

/*
 putChar

 This puts a character at the specified coordinates on the specified window.
 This does not wrefresh!

 Arguments: WINDOW* win - the window to draw a character in
            unsigned int x - x coordinate to draw character at
            unsigned int y - y coordinate to draw character at
            char c - the character to put

 Returns:   None.
*/
void Display::putChar(WindowSPtr w, unsigned int x, unsigned int y, char c) {
    /* Move the window curser then place the character */
    wmove(w, y, x);
    waddch(w, c);
}

/*
 putCharRefresh

 This puts a character at the specified coordinates on the specified window.
 This does wrefresh!

 Arguments: WINDOW* win - the window to draw a character in
            unsigned int x - x coordinate to draw character at
            unsigned int y - y coordinate to draw character at
            char c - the character to put

 Returns:   None.
*/
void Display::putTextChar(unsigned int x, unsigned int y, char c) {
    /* Move the window curser then place the character */
    putChar(_textWindow, y, x, c);
    wrefresh(_textWindow);
}

/*
 drawMap

 This draws the map.

 Arguments: Map *map - the map to draw

 Returns:   None.
*/
void Display::drawMap(std::shared_ptr<Map> map) {
    /* Draw every character from the map. */
    for (int row = 0; row < map->getHeight(); row++) {
        for (int col = 0; col < map->getWidth(); col++) {
            putChar(_mapWindow, col, row,
                map->getCell(col/3, row).raw[col%CELL_SIZE]);
        }
    }

    /* Refresh the window */
    wrefresh(_mapWindow);
}
