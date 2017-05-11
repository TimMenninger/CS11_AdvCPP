#ifndef DISPLAY
#define DISPLAY

#include <ncurses.h>
#include "map.hpp"

class Display {
private:
    WINDOW *_mapWindow;     /* Window containing the game map */
    WINDOW *_textWindow;    /* Window containing text */

public:
    /********************
    CONSTRUCTORS
    ********************/
    Display(Map*);

    /********************
    DESTRUCTORS
    ********************/
    ~Display();

    /********************
    MEMBER FUNCTIONS
    ********************/
    void putChar(WINDOW*, unsigned int, unsigned int, char);
    void putTextChar(unsigned int, unsigned int, char);
    void drawMap(Map*);
};

#endif // ifndef DISPLAY
