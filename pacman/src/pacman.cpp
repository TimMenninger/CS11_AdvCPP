#include "pacman.hpp"

/*
 Pacman

 Constructor, sets everything up to start.

 Arguments: std::shared_ptr<Game> g - the game pacman is playing

 Returns:   None.
*/
Pacman::Pacman(std::shared_ptr<Game> g) :
    _loc(Location(PACMAN_X0, PACMAN_Y0)), _dir(W), _spd(SPD_PACMAN), _game(g) {}

/*
 ~Pacman

 Destructor

 Arguments: None.

 Returns:   None.
*/
Pacman::~Pacman() {
    /* Nothing to delete */
}

/*
 setDirection

 Sets the direction Pacman is traveling.

 Arguments: Direction - new direction

 Returns:   None.
*/
void Pacman::setDirection(Direction d) {
    _dir = d;
}

/*
 getLocation

 Returns Pacman's location in the map.

 Arguments: None.

 Returns:   Location - Pacman's location in the map
*/
Location Pacman::getLocation() {
    return _loc;
}

/*
 getDirection

 Returns Pacman's direction of travel

 Arguments: None.

 Returns:   Location - Pacman's direction of travel
*/
Direction Pacman::getDirection() {
    return _dir;
}

/*
 onStart

 This initializes Pacman (can be called after death).

 Arguments: None.

 Returns:   None.
*/
void Pacman::onStart() {
    /* Set position to beginning */
    _loc = Location(PACMAN_X0, PACMAN_Y0);
    _dir = W;
}

/*
 move

 An infinte loop that perpetually moves pacman in the direction he is
 traveling.  If there is a wall in the next cell in that direction, Pacman
 stays stationary.

 Arguments: None.

 Returns:   None.
*/
void Pacman::move() {
    /* We want to move "forward" until the game is over. */
    while (_game->isRunning) {
        /* Get the next space Pacman wants to go to. */
        Location next(
            _loc.x + (_dir == E) - (_dir == W),
            _loc.y + (_dir == S) - (_dir == N)
        );

        /* Move in the proper direction unless there is a wall. */
        if (!_game->isWall(next.x, next.y))
            _loc = next;
        /* Check if we are at a portal */
        if (_loc == Location(L_PORT_X, L_PORT_Y) && _dir == W)
            _loc = Location(R_PORT_X, R_PORT_Y);
        else if (_loc == Location(R_PORT_X, R_PORT_Y) && _dir == E)
            _loc = Location(L_PORT_X, L_PORT_Y);

        /* Wait for 1/speed seconds so we move at the correct rate */
        usleep(1000000 / _spd);
    }
}
