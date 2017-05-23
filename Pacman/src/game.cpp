#include "game.hpp"
#include <iostream>

/*
 Game

 Constructor for game class.

 Arguments: const char *filename - contains the map file.

 Returns:   None.
*/
Game::Game(const char *filename) {
    assert(filename);
    _map        = std::shared_ptr<Map>(new Map(filename));
    _display    = std::shared_ptr<Display>(new Display());
    _textWindow = _display->createWindow(0, 0, COLS, LINES - _map->getHeight());
    _mapWindow  = _display->createWindow(0, LINES - _map->getHeight(), COLS, LINES);

    /* Start with zero score and nothing eaten.  Total dots is filled later */
    dotsEaten = 0;
    dotsLeft = 0;

    /* Put the ghosts into an array. */
    std::shared_ptr<Game> thisGame(this);
    ghosts[IDX_BLINKY] = std::shared_ptr<Blinky>(new Blinky(thisGame));
    ghosts[IDX_PINKY]  = std::shared_ptr<Pinky>(new Pinky(thisGame));
    ghosts[IDX_INKY]   = std::shared_ptr<Inky>(new Inky(thisGame));
    ghosts[IDX_CLYDE]  = std::shared_ptr<Clyde>(new Clyde(thisGame));
    /* Pacman */
    pacman = std::shared_ptr<Pacman>(new Pacman(thisGame));
    /* The game isn't running yet. */
    isRunning = false;
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
 cellOccupiedByGhost

 Checks if any ghosts are on the argued spot

 Arguments: int x - x coordinate of spot to check
            int y - y coordinate of spot to check

 Returns:   bool - true if there is a ghost at this cell
*/
bool Game::cellOccupiedByGhost(int x, int y) {
    bool occupied = false;
    for (int i = 0; i < 4; i++)
        occupied |= (ghosts[i]->getLocation().x == x && ghosts[i]->getLocation().y == y);

    return occupied;
}

/*
 run

 Runs the game.

 Arguments: None.

 Returns:   None.
*/
void Game::run() {
    /* Draw the map */
    drawMap();

    /* Run the game. */
    isRunning = true;

    ghosts[IDX_BLINKY]->move();

    int x = 0;
    while (1) {
        /* Temporary just to show functionality. */
        char c = getch();
        if (c == 27)
            break;
        else
            _display->putChar(_textWindow, (x / 80), (x % 80), c);
        x++;
    }
}

/*
 isWall

 Returns whether or not the argued index is a wall.

 Arguments: int x - x coordinate of cell to check
            int y - y coordinate of cell to check

 Returns:   bool - true if there is a wall at (x, y)
*/
bool Game::isWall(int x, int y) {
    if (x < 0 || y < 0 || x > _map->getWidth() || y > _map->getHeight())
        return true;
    return _map->getCell(x, y).type == WALL;
}

/*
 drawMap

 This draws the map.

 Arguments: Map *map - the map to draw

 Returns:   None.
*/
void Game::drawMap() {
    /* A placeholder for the cell as we iterate through the array. */
    Cell cell;

    /* Draw every character from the map. */
    for (int row = 0; row < _map->getHeight(); row++) {
        for (int col = 0; col < _map->getWidth(); col += 3) {
            /* Get the cell this corresponds to */
            cell = _map->getCell(col/3, row);

            /* Set the display */
            for (int i = 0; i < CELL_SIZE; i++)
                _display->putChar(_mapWindow, col+i, row, cell.raw[i], 0);

            /* Accumulate number of dots into the counter of what is left */
            dotsLeft += (cell.type == DOT || cell.type == POWERUP);
        }
    }

    /* Refresh the window */
    _display->refreshWindow(_mapWindow);
}
