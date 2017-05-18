#ifndef GAME
#define GAME

#include <assert.h>
#include "display.hpp"
#include "map.hpp"

class Game {
private:
    std::shared_ptr<Map> _map;      /* The game map */
    std::unique_ptr<Display> _disp; /* The dipslay */

public:
    /********************
    CONSTRUCTORS
    ********************/
    Game(const char *filename);

    /********************
    DESTRUCTORS
    ********************/
    ~Game();

    /********************
    MEMBER FUNCTIONS
    ********************/
    void run();
};

#endif // ifndef GAME
