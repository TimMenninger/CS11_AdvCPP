#ifndef GAME
#define GAME

#include <assert.h>
#include <thread>
#include <ctime>
#include "display.hpp"
#include "map.hpp"
#include "common.hpp"
#include "ghost.hpp"
#include "pacman.hpp"

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
    int _score;                         /* Player's score */
    int _lives;                         /* Lives left */

    clock_t frightenedTime;             /* How many seconds left as frightened */
    int numGhostsEaten;                 /* Ghosts eaten during powerup */

    int secs;                           /* Total seconds of gameplay */

    void drawMap(bool=false);           /* Draws just the map */
    void keyListener();                 /* Listens for user input keys */
    void updateMapCharacters();         /* Updates map with characters */
    void updateDots();                  /* Handles Pacman eating dots */
    void updateGhostStates();           /* Updates the states of ghosts */
    void checkCollisions();             /* Checks and updates pac/ghost collisions */
    void updateStats();                 /* Prints score and lives at top */

public:
    /* The ghosts. Public so they can find each other */
    Ghost **ghosts;
    /* Pacman public so ghosts can access his location */
    std::shared_ptr<Pacman> pacman;
    /* Will be true until the game is no longer running. */
    bool isRunning;
    /* Dots eaten and dots left for ghosts to act upon. */
    int dotsEaten;
    int dotsLeft;
    GhostState ghostState;

    /********************
    CONSTRUCTORS
    ********************/
    Game();

    /********************
    DESTRUCTORS
    ********************/
    ~Game();

    /********************
    MEMBER FUNCTIONS
    ********************/
    void run();
    bool isWall(int, int);
    bool cellOccupiedByGhost(int, int);
};

#endif // ifndef GAME
