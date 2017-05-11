#include "game.hpp"

/*
 Game

 Constructor for game class.

 Arguments: const char *filename - contains the map file.

 Returns:   None.
*/
Game::Game(const char *filename) {
    assert(filename);
    _map = new Map(filename);
    _disp = new Display(_map);
}

/*
 ~Game

 Destructor for game class.

 Arguments: None.

 Returns:   None.
*/
Game::~Game() {
    if (_map) delete _map;
    if (_disp) delete _disp;
}

/*
 run

 Runs the game.

 Arguments: None.

 Returns:   None.
*/
void Game::run() {
    int x = 0;
    while (1) {
        /* Temporary just to show functionality. */
        char c = getch();
        if (c == 27)
            break;
        else
            _disp->putTextChar((x / 80), (x % 80), c);
        x++;
    }
}
