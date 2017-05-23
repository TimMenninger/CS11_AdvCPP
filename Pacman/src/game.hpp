#ifndef GAME
#define GAME

#include <assert.h>
#include "display.hpp"
#include "map.hpp"
#include "common.hpp"
#include "ghost.hpp"
#include "pacman.hpp"

/* Indices in the ghost array of the four ghosts. */
#define IDX_BLINKY  0
#define IDX_PINKY   1
#define IDX_INKY    2
#define IDX_CLYDE   3

/* Speeds of the ghosts and Pacman in cells/second */
#define SPD_PACMAN  3
#define SPD_BLINKY  3.2
#define SPD_PINKY   3
#define SPD_INKY    3
#define SPD_CLYDE   2.8

/* Common values for the ghost house */
#define GHOSTHOUSE_XL   11      /* The X coordinate of left ghost house spot */
#define GHOSTHOUSE_XC   13      /* The X coordinate of center ghost house spot */
#define GHOSTHOUSE_XR   15      /* The X coordinate of right ghost house spot */
#define GHOSTHOUSE_Y    15      /* The Y coordinate of the ghost house */
#define GHOST_EXIT_X    13      /* The X coordinate of first cell outside */
#define GHOST_EXIT_Y    15      /* The Y coordinate of first cell outside */

/* Ghost start locations */
#define BLINKY_X0       13
#define BLINKY_Y0       12
#define PINKY_X0        13
#define PINKY_Y0        15
#define INKY_X0         11
#define INKY_Y0         15
#define CLYDE_X0        15
#define CLYDE_Y0        15

/* Ghost scatter locations */
#define BLINKY_SCAT_X   29
#define BLINKY_SCAT_Y   -1
#define PINKY_SCAT_X    -1
#define PINKY_SCAT_Y    -1
#define INKY_SCAT_X     29
#define INKY_SCAT_Y     29
#define CLYDE_SCAT_X    -1
#define CLYDE_SCAT_Y    29

/* Pacman starting location */
#define PACMAN_X0       13
#define PACMAN_Y0       25

/* Put this here so we can use it in Game */
class Ghost;
class Pacman;

/* This class runs the game */
class Game {
private:
    std::shared_ptr<Map> _map;          /* The game map */
    std::shared_ptr<Display> _display;  /* The ncurses display object */
    WindowSPtr _textWindow;             /* ncurses window for text */
    WindowSPtr _mapWindow;              /* ncurses window for map */

public:
    /* The ghosts. Public so they can find each other */
    std::shared_ptr<Ghost> ghosts[4];
    /* Pacman public so ghosts can access his location */
    std::shared_ptr<Pacman> pacman;
    /* Will be true until the game is no longer running. */
    bool isRunning;
    /* Dots eaten and dots left for ghosts to act upon. */
    int dotsEaten;
    int dotsLeft;

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
    void drawMap();
    bool isWall(int, int);
    bool cellOccupiedByGhost(int, int);
};

#endif // ifndef GAME
