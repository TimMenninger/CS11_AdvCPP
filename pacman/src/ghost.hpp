#ifndef GHOSTS
#define GHOSTS

#include "common.hpp"
#include "game.hpp"
#include <unistd.h>

/* The directions returned by getPossibleDirections */
#define _N 0x1
#define _E 0x2
#define _S 0x4
#define _W 0x8

/* Declare this here so we can use it in our Ghost */
class Game;

/* The ghost class is the parent of the four individual ghosts in Pacman;
   Inky, Blinky, Pinky and Clyde */
class Ghost {
protected:
    Location    _loc;   /* Location of the ghost. */
    Direction   _dir;   /* Direction of travel. */
    double      _spd;   /* The speed of the ghost (Pacman's is always 1) */
    GhostState  _st;    /* The state the ghost is currently in. */
    Location    _tgt;   /* The target cell of the ghost. */

    std::shared_ptr<Game> _game; /* The game this is a part of */

    bool        _rstrt; /* When true, we want to run onStart. We need a flag
                           because we can't have a thread call functions that
                           block (onStart's block) and the wait in the moves
                           are too quick to set a state */

    /* Returns the directions the ghost can move given its location,
       direction and state. */
    char getPossibleMoves();

    /* Update teh location based on the target, state and direction */
    void updateLocation();

public:
    /********************
    CONSTRUCTORS
    ********************/
    explicit Ghost(std::shared_ptr<Game>);

    /********************
    DESTRUCTORS
    ********************/
    virtual ~Ghost();

    /********************
    MEMBER FUNCTIONS
    ********************/
    Location getLocation();
    Location getTarget();
    void setState(GhostState);
    GhostState getState();
    void onDeath();
    void restart() { _rstrt = true; };

    /********************
    INTERFACE FUNCTIONS
    ********************/
    virtual void onStart() {} ;
    virtual void move() {};
};

/* The individual ghost classes implement onStart and move for the parent
   Ghost class. */
class Inky : public Ghost {
public:
    /* Constructor */
    explicit Inky(std::shared_ptr<Game> g) : Ghost(g) {};
    /* Destructor */
    ~Inky() {};
    /* Interface implementation */
    void onStart();
    void move();
};

class Blinky : public Ghost {
public:
    /* Constructor */
    explicit Blinky(std::shared_ptr<Game> g) : Ghost(g) {};
    /* Destructor */
    ~Blinky() {};
    /* Interface implementation */
    void onStart();
    void move();
};

class Pinky : public Ghost {
public:
    /* Constructor */
    explicit Pinky(std::shared_ptr<Game> g) : Ghost(g) {};
    /* Destructor */
    ~Pinky() {};
    /* Interface implementation */
    void onStart();
    void move();
};

class Clyde : public Ghost {
public:
    /* Constructor */
    explicit Clyde(std::shared_ptr<Game> g) : Ghost(g) {};
    /* Destructor */
    ~Clyde() {};
    /* Interface implementation */
    void onStart();
    void move();
};

#endif // ifndef GHOSTS
