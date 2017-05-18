#include "game.hpp"

/*
 Game

 Constructor for game class.

 Arguments: const char *filename - contains the map file.

 Returns:   None.
*/
Game::Game(const char *filename) {
    assert(filename);
    _map = std::shared_ptr<Map>(new Map(filename));
    _disp = std::unique_ptr<Display>(new Display(_map));
}

/*
 ~Game

 Destructor for game class.

 Arguments: None.

 Returns:   None.
*/
Game::~Game() {
    /* Nothing to delete when using smart pointers */
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
