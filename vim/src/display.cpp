#include "display.hpp"

using namespace std;

/*
 Display

 Constructor.  This initializes the screen and windows.

 Arguments: None.

 Returns:   None.
*/
Display::Display() {
    /* Initialize the screen */
    initscr();

    /* Set options */
    cbreak();
    noecho();

    /* Use keyboard */
    raw();
    keypad(stdscr, TRUE);
}

/*
 ~Display

 Destructor.  This cleans up the screen and window.

 Arguments: None.

 Returns:   None.
*/
Display::~Display() {
    /* End the session.  The window smart pointers will delete the windows */
    endwin();
}

/*
 createWindow

 This creates a window and adds it to the vector of windows.

 Arguments: unsigned int x1 - x coordinate of top left corner
            unsigned int y1 - y coordinate of top left corner
            unsigned int x2 - x coordinate of bottom left corner
            unsigned int y2 - y coordinate of bottom left corner

 Returns:   WindowSPtr - a smart pointer to the window
*/
WindowSPtr Display::createWindow(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    /* Create the new window and return it. */
    WindowSPtr w(newwin(y2-y1, x2-x1, y1, x1));
    windows.push_back(w);
    refresh();
    return w;
}

/*
 putChar

 This puts a character at the specified coordinates on the specified window.
 This will refresh unless explicitly said not to.

 Arguments: WINDOW* win - the window to draw a character in
            unsigned int x - x coordinate to draw character at
            unsigned int y - y coordinate to draw character at
            char c - the character to put
            bool rfsh - if true, this should refresh the window, and has a
                default value of true

 Returns:   None.
*/
void Display::putChar(WindowSPtr w, unsigned int x, unsigned int y, char c, bool rfsh) {
    /* Move the window curser then place the character */
    wmove(w, y, x);
    waddch(w, c);

    /* Only refresh if asked to or not specified */
    if (rfsh)
        wrefresh(w);
}

/*
 putLine

 This puts a string at the argued line and then wrefresh's.

 Arguments: WINDOW* win - the window to draw a character in
            unsigned int y - the line to put the line at
            char c - the character to put
            bool rfsh - if true, refreshes the window

 Returns:   None.
*/
void Display::putLine(WindowSPtr w, unsigned int y, char *line, bool rfsh) {
    /* Clear the line before writing */
    wmove(w, y, 0);
    wclrtoeol(w);

    for (int x = 0; *line != 0 && x < COLS; line++)
        /* Move the window curser then place the character */
        putChar(w, x++, y, *line, false);

    if (rfsh)
        wrefresh(w);
}

/*
 refreshWindow

 Refreshes a window.  This is here so external users don't need to use
 ncurses functions.

 Arguments: WindowSPtr w - the window to refresh

 Returns:   None.
*/
void Display::refreshWindow(WindowSPtr w) {
    wrefresh(w);
}
