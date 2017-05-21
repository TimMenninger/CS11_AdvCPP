#ifndef DISPLAY
#define DISPLAY

#include <ncurses.h>
#include "map.hpp"

class WindowSPtr {
private:
    WINDOW *win;            /* The WINDOW this class wraps */
    int    *nRefs;          /* Number of references to this WINDOW */

public:
    /********************
    CONSTRUCTORS
    ********************/
    WindowSPtr() : win(nullptr), nRefs(nullptr) {};
    WindowSPtr(WINDOW *wptr) : win(wptr) {
        nRefs = new int(1);
    };

    /* Copy constructor adds to reference count */
    WindowSPtr(const WindowSPtr& wptr) : win(wptr.win), nRefs(wptr.nRefs) {
        (*nRefs)++;
    };
    /* Move constructor keeps reference count same */
    WindowSPtr(WindowSPtr&& wptr) : win(wptr.win), nRefs(wptr.nRefs) {
        memset((void *) &wptr, 0, sizeof(WindowSPtr));
    };

    /********************
    DESTRUCTORS
    ********************/
    ~WindowSPtr() {
        /* Remove this from reference count, and only delete window if
           that brings reference count to zero. */
        if (nRefs) {
            (*nRefs)--;
            if (*nRefs <= 0) {
                delwin(win);
            }
        }
    };

    /********************
    OPERATORS
    ********************/
    /* Copy assignment */
    WindowSPtr operator=(const WindowSPtr& wptr) {
        /* We will seg fault if we try to move into itself. */
        if (this != &wptr) {
            /* Delete pointers and ref count if this overwrites other
               pointers. */
            if (nRefs && nRefs != wptr.nRefs) delete nRefs;
            if (win && win != wptr.win) this->~WindowSPtr();
            /* Perform the operation */
            memcpy((void *) this, (void *) &wptr, sizeof(WindowSPtr));
            (*nRefs)++;
        }
        return *this;
    };
    /* Move assignment */
    WindowSPtr operator=(WindowSPtr&& wptr) {
        /* We will seg fault if we try to move into itself. */
        if (this != &wptr) {
            /* Delete pointers and ref count if this overwrites other
               pointers. */
            if (nRefs && nRefs != wptr.nRefs) delete nRefs;
            if (win && win != wptr.win) this->~WindowSPtr();
            /* Perform the operation */
            memcpy((void *) this, (void *) &wptr, sizeof(WindowSPtr));
            memset((void *) &wptr, 0, sizeof(WindowSPtr));
        }
        return *this;
    };
    /* Implicit cast to WINDOW* raw pointer */
    const operator WINDOW*() {
        return win;
    };
};

class Display {
private:
    WindowSPtr _mapWindow;     /* Window containing the game map */
    WindowSPtr _textWindow;    /* Window containing text */

public:
    /********************
    CONSTRUCTORS
    ********************/
    Display(std::shared_ptr<Map>);

    /********************
    DESTRUCTORS
    ********************/
    ~Display();

    /********************
    MEMBER FUNCTIONS
    ********************/
    void putChar(WindowSPtr, unsigned int, unsigned int, char);
    void putTextChar(unsigned int, unsigned int, char);
    void drawMap(std::shared_ptr<Map> Map);
};

#endif // ifndef DISPLAY
