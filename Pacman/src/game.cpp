#include "game.hpp"

using namespace std;

/*
 Game

 Constructor for game class.

 Arguments: None.

 Returns:   None.
*/
Game::Game() {
    _map        = std::shared_ptr<Map>(new Map(MAP_FILE));
    _display    = std::shared_ptr<Display>(new Display());
    _textWindow = _display->createWindow(0, 0, COLS, LINES - _map->getHeight());
    _mapWindow  = _display->createWindow(0, LINES - _map->getHeight(), COLS, LINES);

    /* Start with zero score and nothing eaten.  Total dots is filled later */
    dotsEaten = 0;
    dotsLeft = 0;

    /* Score initially 0 with 3 lives */
    _score = 0;
    _lives = 3;

    /* Ghosts are initially not frightened */
    frightenedTime = clock();

    /* Not in powerup mode, so no ghosts eaten yet */
    numGhostsEaten = 0;

    /* Ghosts start in scatter state */
    ghostState = SCATTER;

    /* Put the ghosts into an array. */
    std::shared_ptr<Game> thisGame(this);
    ghosts = (Ghost **) malloc(4 * sizeof(Ghost*));
    ghosts[IDX_BLINKY] = new Blinky(thisGame);
    ghosts[IDX_PINKY]  = new Pinky(thisGame);
    ghosts[IDX_INKY]   = new Inky(thisGame);
    ghosts[IDX_CLYDE]  = new Clyde(thisGame);
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
    /* Delete our array of ghosts (and everything in it) */
    delete[] ghosts;
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
        occupied |= (ghosts[i]->getLocation() == Location(x, y));

    return occupied;
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

 Arguments: bool countDots - defaults to false. When true, counts how many
                dots are left.

 Returns:   None.
*/
void Game::drawMap(bool countDots) {
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
            if (countDots)
                dotsLeft += (cell.type == DOT || cell.type == POWERUP);
        }
    }

    /* Refresh the window */
    _display->refreshWindow(_mapWindow);
}

/*
 keyListener

 This listens for user input keys and changes Pacman's direction accordingly.

 Arguments: None.

 Returns:   None.
*/
void Game::keyListener() {
    while (isRunning) {
        /* Temporary just to show functionality. */
        int c = getch();
        if (c == 27) {
            isRunning = false;
            return;
        }
        else if (c == KEY_UP)
            pacman->setDirection(N);
        else if (c == KEY_DOWN)
            pacman->setDirection(S);
        else if (c == KEY_LEFT)
            pacman->setDirection(W);
        else if (c == KEY_RIGHT)
            pacman->setDirection(E);
    }
}

/*
 updateMapCharacters

 This updates the map by drawing the raw map (according to _map, so dots will
 not appear if they've been eaten), then puts all of the characters on it.

 Arguments: None.

 Returns:   None.
*/
void Game::updateMapCharacters() {
    /* Draw the map */
    drawMap();

    /* Draw all of the characters with their appropriate symbol. We use
       & for pacman, # for ghost, ! for frightened ghost */
    Location pacLoc = pacman->getLocation();
    _display->putChar(_mapWindow, 3*pacLoc.x + 1, pacLoc.y, '&');

    for (int i = 0; i < 4; i++) {
        /* Shorthand */
        GhostState st = ghosts[i]->getState();
        Location l = ghosts[i]->getLocation();

        /* Put the ghost's symbol */
        _display->putChar(
            _mapWindow, 3*l.x + 1, l.y,
            st == FRIGHTENED || st == DYING ? '!' : '#');
    }

    /* Refresh the window to display the updates */
    _display->refreshWindow(_mapWindow);
}

/*
 updateDots

 Considers the map and Pacman's location to update the score and dot counts
 as well as the map to no longer display eaten dots.

 Arguments: None.

 Returns:   None.
*/
void Game::updateDots() {
    /* Shorthand */
    Location pacLoc = pacman->getLocation();
    Cell curr = _map->getCell(pacLoc.x, pacLoc.y);

    /* Now consider if Pacman is on a dot */
    if (curr.type == DOT) {
        /* Remove the dot and add to the score */
        curr.type = NODOT;
        curr.raw[1] = ' ';
        _map->setCell(pacLoc.x, pacLoc.y, curr);

        /* Add to the score and account for the dot eaten */
        _score += 10;
        dotsEaten++;
        dotsLeft--;
    }
    /* Or if he is on a powerup */
    else if (curr.type == POWERUP) {
        /* Remove the dot */
        curr.type = NOPOWERUP;
        curr.raw[1] = ' ';
        _map->setCell(pacLoc.x, pacLoc.y, curr);

        /* Add to score */
        _score += 50;
        dotsEaten++;
        dotsLeft--;

        /* Start the frightened ghosts timer. */
        frightenedTime = clock() + 7*CLOCKS_PER_SEC;

        /* Set all non-jailed ghosts as frightened */
        for (int i = 0; i < 4; i++)
            if (ghosts[i]->getState() != JAIL)
                ghosts[i]->setState(FRIGHTENED);
    }
}

/*
 updateGhostStates

 Updates the states of the ghosts based on how much time has passed.

 Arguments: None.

 Returns:   None.
*/
void Game::updateGhostStates() {
    /* Set ghost state based on time */
    GhostState nextState;
    if (secs < 7)
        nextState = SCATTER;
    else if (secs >= 7 && secs < 27)
        nextState = CHASE;
    else if (secs >= 27 && secs < 34)
        nextState = SCATTER;
    else if (secs >= 34 && secs < 54)
        nextState = CHASE;
    else if (secs >= 54 && secs < 59)
        nextState = SCATTER;
    else if (secs >= 59 && secs < 79)
        nextState = CHASE;
    else if (secs >= 79 && secs < 84)
        nextState = SCATTER;
    else
        nextState = CHASE;

    /* Now set the state based on what was determined from the time */
    for (int i = 0; i < 4; i++) {
        GhostState st = ghosts[i]->getState();
        /* If the ghost is in jail, stay there */
        if (st == JAIL || st == DYING)
            continue;

        /* If no frightened counter, definitely set state */
        if (frightenedTime < clock()) {
            /* Reset the number of ghosts eaten */
            numGhostsEaten = 0;

            /* Use intermediate scatter on state change */
            if (nextState == SCATTER && (st == CHASE || st == SCATTER_INT))
                ghosts[i]->setState(SCATTER_INT);
            else
                ghosts[i]->setState(nextState);
        }
    }
}

/*
 checkCollisions

 This checks if Pacman has collided with a ghost.  If he has, then it handles
 accordingly depending on whether the ghost was frightened or not.

 Arguments: None.

 Returns:   None.
*/
void Game::checkCollisions() {
    /* Shorthand */
    Location pacLoc = pacman->getLocation();

    /* Check if pacman and ghost collide */
    for (int i = 0; i < 4; i++) {
        GhostState st = ghosts[i]->getState();
        if (pacLoc == ghosts[i]->getLocation()) {
            if (st == FRIGHTENED) {
                /* Count this in the score */
                _score += 200 * (1 << numGhostsEaten++);

                /* Move back to jail */
                ghosts[i]->setState(DYING);
                ghosts[i]->onDeath();
            }
            else if (st != DYING && st != RESTART) {
                /* Start over */
                for (int j = 0; j < 4; j++)
                    ghosts[j]->restart();
                pacman->onStart();

                /* Remove a life */
                _lives--;
            }
        }
    }
}

/*
 updateStats

 Prints the score and lives left at the top of the screen in the text window.

 Arguments: None.

 Returns:   None.
*/
void Game::updateStats() {
    /* Print the score at the top */
    _display->putLine(_textWindow, 0,
        (char *) (string("score: ") + to_string(_score)).c_str());
    _display->putLine(_textWindow, 1,
        (char *) (string("lives: ") + to_string(_lives)).c_str());
}

/*
 run

 Runs the game.

 Arguments: None.

 Returns:   None.
*/
void Game::run() {
    /* Draw the map */
    drawMap(true);

    /* Run the game. */
    isRunning = true;

    /* Start threads for the ghosts */
    thread tBlinky(&Ghost::move, ghosts[IDX_BLINKY]);
    thread tInky(&Ghost::move, ghosts[IDX_INKY]);
    thread tPinky(&Ghost::move, ghosts[IDX_PINKY]);
    thread tClyde(&Ghost::move, ghosts[IDX_CLYDE]);
    thread tPacman(&Pacman::move, pacman);
    thread keyListener(&Game::keyListener, this);

    /* Start a clock for the timer */
    clock_t start = clock();
    while(isRunning && _lives > 0 && dotsLeft > 0) {
        /* Draw the map and all of the characters in it */
        updateMapCharacters();

        /* Update the dots and counts based on Pacman's location */
        updateDots();

        /* Update ghosts' states */
        updateGhostStates();

        /* Check and update according to if pacman collided with ghost */
        checkCollisions();

        /* Display score and lives left */
        updateStats();

        /* Advance the timers */
        secs = (clock() - start) / CLOCKS_PER_SEC;

        /* Just to limit processing */
        usleep(100);
    }

    /* If here, we either quit or lost lives, but make sure that we are no
       longer running */
    isRunning = false;

    /* Print statistics */
    _display->putLine(_textWindow, 0,
        (char *) (string("Game over! Final score: ") + to_string(_score)).c_str());
    _display->putLine(_textWindow, 1,
        (char *) string("Press any key to exit.").c_str());

    /* Join all of the threads */
    tBlinky.join();
    tInky.join();
    tPinky.join();
    tClyde.join();
    tPacman.join();
    keyListener.join();
}
