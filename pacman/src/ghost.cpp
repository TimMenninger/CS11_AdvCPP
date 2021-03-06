#include "ghost.hpp"

/**********************************************************************

GHOST CLASS

**********************************************************************/

/*
 Ghost

 Constructor, sets everything empty so children can set it.

 Arguments: std::shared_ptr<Ghost[]> g - array of all ghosts
            std::shared_ptr<Pacman> p - pacman

 Returns:   None.
*/
Ghost::Ghost(std::shared_ptr<Game> g) {
    _loc = Location(0, 0);
    _dir = N;
    _spd = 0;
    _st  = JAIL;
    _tgt = Location(0, 0);

    _game = g;
}

/*
 ~Ghost

 Destructor

 Arguments: None.

 Returns:   None.
*/
Ghost::~Ghost() {
    /* Nothing to delete because we use only smart pointers */
}

/*
 getPossibleMoves

 Returns a char describing the directions the ghost can move to at
 this point.

 Arguments: None.

 Returns:   char - a bitmap of directions the ghost can move
*/
char Ghost::getPossibleMoves() {
    char moves = 0;

    /* Conditions for being able to move W */
    if (!_game->isWall(_loc.x-1, _loc.y)) {
        if (_dir == E && _st == SCATTER_INT) {
            _st = SCATTER; /* Only want to be able to do this once */
            moves |= _W;
        }
        else if (_dir != E) {
            moves |= _W;
        }
    }

    /* Conditions for being able to move N */
    if (!_game->isWall(_loc.x, _loc.y-1) &&
        _loc != Location(NO_UP_X1, NO_UP_Y1) &&
        _loc != Location(NO_UP_X2, NO_UP_Y2) &&
        _loc != Location(NO_UP_X3, NO_UP_Y3) &&
        _loc != Location(NO_UP_X4, NO_UP_Y4)
    ) {
        if (_dir == S && _st == SCATTER_INT) {
            _st = SCATTER; /* Only want this to happen once */
            moves |= _N;
        }
        else if (_dir != S) {
            moves |= _N;
        }
    }

    /* Conditions for being able to move S */
    if (!_game->isWall(_loc.x, _loc.y+1)) {
        if (_dir == N && _st == SCATTER_INT) {
            _st = SCATTER; /* Only want to be able to do this once */
            moves |= _S;
        }
        else if (_dir != N) {
            moves |= _S;
        }
    }

    /* Conditions for being able to move E */
    if (!_game->isWall(_loc.x+1, _loc.y)) {
        if (_dir == W && _st == SCATTER_INT) {
            _st = SCATTER; /* Only want this to happen once */
            moves |= _E;
        }
        else if (_dir != W) {
            moves |= _E;
        }
    }

    return moves;
}

/*
 updateLocation

 Takes the target and decides where to move next.  This checks what
 directions the ghost is allowed to move, and tests in order of
 priority to self-handle the equidistant case.

 Arguments: None.

 Returns:   None.
*/
void Ghost::updateLocation() {
    /* Get possible moves. */
    char moves = getPossibleMoves();

    /* Keep track of the shortest distance and corresponding location */
    double bestDist = -1;
    Location bestLoc = _loc;
    Direction bestDir = _dir;
    /* Keep track of how many directions we've considered.  This allows
       us to properly randomly choose a direction without counting how
       many possible directions befrehand. */
    int dirs = 0;

    if (moves & _N) {
        bestDist = linDist(_tgt, Location(_loc.x, _loc.y-1));

        bestLoc = Location(_loc.x, _loc.y-1);
        bestDir = N;
        dirs++;
    }
    if (moves & _W) {
        double dist = linDist(_tgt, Location(_loc.x-1, _loc.y));

        if (bestDist < 0 || dist < bestDist) {
            bestDist = dist;
            bestLoc = Location(_loc.x-1, _loc.y);
            bestDir = W;
        }
        /* In the case that we are in frightened mode, we take it with
           chance 1/n where this is the n'th possible direction.  This
           will always result with equal chance of all directions */
        if (_st == FRIGHTENED && ((float) rand()/RAND_MAX < 1/++dirs)) {
            bestLoc = Location(_loc.x-1, _loc.y);
            bestDir = W;
        }
    }
    if (moves & _S) {
        double dist = linDist(_tgt, Location(_loc.x, _loc.y+1));

        if (bestDist < 0 || dist < bestDist) {
            bestDist = dist;
            bestLoc = Location(_loc.x, _loc.y+1);
            bestDir = S;
        }
        /* In the case that we are in frightened mode, we take it with
           chance 1/n where this is the n'th possible direction.  This
           will always result with equal chance of all directions */
        if (_st == FRIGHTENED && ((float) rand()/RAND_MAX < 1/++dirs)) {
            bestLoc = Location(_loc.x, _loc.y+1);
            bestDir = S;
        }
    }
    if (moves & _E) {
        double dist = linDist(_tgt, Location(_loc.x+1, _loc.y));

        if (bestDist < 0 || dist < bestDist) {
            bestDist = dist;
            bestLoc = Location(_loc.x+1, _loc.y);
            bestDir = E;
        }
        /* In the case that we are in frightened mode, we take it with
           chance 1/n where this is the n'th possible direction.  This
           will always result with equal chance of all directions */
        if (_st == FRIGHTENED && ((float) rand()/RAND_MAX < 1/++dirs)) {
            bestLoc = Location(_loc.x+1, _loc.y);
            bestDir = E;
        }
    }

    /* Check if we are at portal */
    if (_loc == Location(L_PORT_X, L_PORT_Y) && bestDir == W)
        bestLoc = Location(R_PORT_X, R_PORT_Y);
    else if (_loc == Location(R_PORT_X, R_PORT_Y) && bestDir == E)
        bestLoc = Location(L_PORT_X, L_PORT_Y);

    /* Update the location accordingly */
    _loc = bestLoc;
    _dir = bestDir;
}

/*
 setState

 Sets the state.  This is here so we can set the state of all ghosts
 at the same time.

 Arguments: GhostState state - the new state of the ghost

 Returns:   None.
*/
void Ghost::setState(GhostState state) {
    _st = state;
}

/*
 getState

 Gets the ghost's state.

 Arguments: None.

 Returns:   GhostState - the state of the ghost
*/
GhostState Ghost::getState() {
    return _st;
}

/*
 getLocation

 Returns the location of the ghost.

 Arguments: None.

 Returns:   Location - the location of the ghost
*/
Location Ghost::getLocation() {
    return _loc;
}

/*
 getTarget

 Returns the target cell of the ghost.

 Arguments: None.

 Returns:   Location - the target of the ghost
*/
Location Ghost::getTarget() {
    return _tgt;
}

/*
 onDeath

 This dictates what happens when a ghost dies.  It checks where the
 other ghosts are and then sets a target to one of the ghost house
 cells that is unoccupied.

 Arguments: None.

 Returns:   None.
*/
void Ghost::onDeath() {
    /* There are three possible places in the cell: left, center and
       right.  We will try to fill up in order from left to right.
       We will check the location of each ghost, and mark from false
       to true if they are in one of these cells. */
    _tgt = Location(GHOST_EXIT_X, GHOST_EXIT_Y);
}

/**********************************************************************

GHOST IMPLEMENTATIONS

**********************************************************************/

/***** BLINKY *****/
void Blinky::onStart() {
    /* No longer need to restart */
    _rstrt = false;

    /* Blinky starts outside the cell, so this will just set the
       location and return. */
    _loc = Location(BLINKY_X0, BLINKY_Y0);
    _spd = SPD_BLINKY;
    _st  = _game->ghostState;
}

void Blinky::move() {
    /* This calls onStart, which won't return until we are supposed
       to actually move. */
    onStart();

    /* Move until the game is over. */
    while (_game->isRunning) {
        /* If we are at start condition, run onStart to start over because
           Pacman died */
        if (_rstrt)
            this->onStart();

        /* Get Pacman's location and direction for simplicity */
        Location pac = _game->pacman->getLocation();

        /* Define the target cell based on state. */
        if (_st == CHASE) {
            _tgt = pac;
        }
        else if (_st == SCATTER || _st == SCATTER_INT) {
            _tgt = Location(BLINKY_SCAT_X, BLINKY_SCAT_Y);
        }
        else if (_st == DYING && _tgt == _loc) {
            _st = _game->ghostState;
        }

        /* Update the location based on the target, state, etc */
        updateLocation();

        /* Wait 1/speed seconds to emulate proper movement */
        usleep((1 - (0.5 * (_st == DYING))) * 1000000 / _spd / (1 - (_st == FRIGHTENED) * 0.4));
    }
}

/***** PINKY *****/
void Pinky::onStart() {
    /* No longer need to restart */
    _rstrt = false;

    /* Starts inside the ghost house and leaves immediately */
    _loc = Location(PINKY_X0, PINKY_Y0);
    _spd = SPD_PINKY;
    _st = JAIL;

    /* Wait minimum one second so we don't overlap blinky */
    usleep(1000000);

    /* Wait until blinky is no longer at the starting spot. */
    while (_game->cellOccupiedByGhost(GHOST_EXIT_X, GHOST_EXIT_Y)) {
        usleep(1000);
    }

    /* Location is now outside the ghost cell */
    _loc = Location(GHOST_EXIT_X, GHOST_EXIT_Y);

    /* Set state */
    _st = _game->ghostState;
}

void Pinky::move() {
    /* This calls onStart, which won't return until we are supposed
       to actually move. */
    onStart();

    /* Move until the game is over. */
    while (_game->isRunning) {
        /* If we are at start condition, run onStart to start over because
           Pacman died */
        if (_rstrt)
            this->onStart();

        /* Get Pacman's location and direction for simplicity */
        Location pac = _game->pacman->getLocation();
        Direction pacD = _game->pacman->getDirection();

        /* Define the target cell based on state. */
        if (_st == CHASE) {
            _tgt.x = pac.x + 4 * ((pacD == E) - (pacD == W));
            _tgt.y = pac.y + 4 * ((pacD == S) - (pacD == N));
        }
        else if (_st == SCATTER || _st == SCATTER_INT) {
            _tgt = Location(PINKY_SCAT_X, PINKY_SCAT_Y);
        }
        else if (_st == DYING && _tgt == _loc) {
            _st = _game->ghostState;
        }

        /* Update the location based on the target, state, etc */
        updateLocation();

        /* Wait 1/speed seconds to emulate proper movement */
        usleep((1 - (0.5 * (_st == DYING))) * 1000000 / _spd / (1 - (_st == FRIGHTENED) * 0.4));
    }
}

/***** INKY *****/
void Inky::onStart() {
    /* No longer need to restart */
    _rstrt = false;

    /* Starts inside the ghost house until Pacman eats 30 dots */
    _loc = Location(INKY_X0, INKY_Y0);
    _spd = SPD_INKY;
    _st = JAIL;

    /* Wait minimum 2 seconds so it doesn't overlap pinky */
    usleep(2000000);
    while (_game->dotsEaten < 30 && _game->isRunning) {
        usleep(1000);
    }

    _st = _game->ghostState;
    _loc = Location(GHOST_EXIT_X, GHOST_EXIT_Y);
}

void Inky::move() {
    /* This calls onStart, which won't return until we are supposed
       to actually move. */
    onStart();

    /* Move until the game is over. */
    while (_game->isRunning) {
        /* If we are at start condition, run onStart to start over because
           Pacman died */
        if (_rstrt)
            this->onStart();

        /* Get Pacman's location and direction for simplicity */
        Location pac = _game->pacman->getLocation();
        Direction pacD = _game->pacman->getDirection();

        /* Define the target cell based on state. */
        if (_st == CHASE) {
            /* We want to get the space two ahead of Pacman, and
               create a vector from Blinky to it.  We then add twice
               that vector to inky's current location. */
            Location pointA = _game->ghosts[IDX_BLINKY]->getLocation();
            Location pointB;
            pointB.x = pac.x + 2 * ((pacD == E) - (pacD == W));
            pointB.y = pac.y + 2 * ((pacD == S) - (pacD == N));

            _tgt.x = _loc.x + 2 * (pointB.x - pointA.x);
            _tgt.y = _loc.y + 2 * (pointB.y - pointA.y);
        }
        else if (_st == SCATTER || _st == SCATTER_INT) {
            _tgt = Location(INKY_SCAT_X, INKY_SCAT_Y);
        }
        else if (_st == DYING && _tgt == _loc) {
            _st = _game->ghostState;
        }

        /* Update the location based on the target, state, etc */
        updateLocation();

        /* Wait 1/speed seconds to emulate proper movement */
        usleep((1 - (0.5 * (_st == DYING))) * 1000000 / _spd / (1 - (_st == FRIGHTENED) * 0.4));
    }
}

/***** CLYDE *****/
void Clyde::onStart() {
    /* No longer need to restart */
    _rstrt = false;

    /* Starts inside the ghost house and waits until Pacman has
       eaten at least 1/3 of the dots. */
    _loc = Location(CLYDE_X0, CLYDE_Y0);
    _spd = SPD_CLYDE;
    _st = JAIL;

    /* Wait minimum 3 seconds so we don't overlap inky */
    usleep(3000000);
    while (_game->dotsEaten <= (float)_game->dotsLeft/2 && _game->isRunning) {
        usleep(1000);
    }

    _st = _game->ghostState;
    _loc = Location(GHOST_EXIT_X, GHOST_EXIT_Y);
}

void Clyde::move() {
    /* This calls onStart, which won't return until we are supposed
       to actually move. */
    onStart();

    /* Move until the game is over. */
    while (_game->isRunning) {
        /* If we are at start condition, run onStart to start over because
           Pacman died */
        if (_rstrt)
            this->onStart();

        /* Get Pacman's location and direction for simplicity */
        Location pac = _game->pacman->getLocation();

        /* Define the target cell based on state. */
        if (_st == CHASE) {
            /* Pursues the cell Pacman is on if > 8 cells away.
               Otherwise goes to scatter location */
            double dist = linDist(pac, _loc);

            if (dist > 8)
                _tgt = pac;
            else
                _tgt = Location(CLYDE_SCAT_X, CLYDE_SCAT_Y);
        }
        else if (_st == SCATTER || _st == SCATTER_INT) {
            _tgt = Location(CLYDE_SCAT_X, CLYDE_SCAT_Y);
        }
        else if (_st == DYING && _tgt == _loc) {
            _st = _game->ghostState;
        }

        /* Update the location based on the target, state, etc */
        updateLocation();

        /* Wait 1/speed seconds to emulate proper movement */
        usleep((1 - (0.5 * (_st == DYING))) * 1000000 / _spd / (1 - (_st == FRIGHTENED) * 0.4));
    }
}
