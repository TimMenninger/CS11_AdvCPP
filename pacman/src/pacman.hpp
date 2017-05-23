#ifndef PACMAN
#define PACMAN

#include "game.hpp"
#include <unistd.h>

/* Declare this here so we can use it in the class */
class Game;

/* This is Pacman, the main character of the game */
class Pacman {
private:
    Location    _loc;   /* Location in the map */
    Direction   _dir;   /* Direction Pacman faces */
    double      _spd;   /* Speed Pacman travels */

    std::shared_ptr<Game> _game;  /* The game Pacman is playing in */

public:
    /********************
    CONSTRUCTORS
    ********************/
    Pacman(std::shared_ptr<Game>);

    /********************
    DESTRUCTORS
    ********************/
    ~Pacman();

    /********************
    ACCESSORS/MUTATORS
    ********************/
    void setDirection(Direction d);
    Location getLocation();
    Direction getDirection();

    /********************
    MEMBER FUNCTIONS
    ********************/
    void move();
};

#endif // ifndef PACMAN
