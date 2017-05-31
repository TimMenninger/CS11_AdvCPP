#ifndef COMMON
#define COMMON

#include <math.h>

/* Indices in the ghost array of the four ghosts. */
#define IDX_BLINKY      0
#define IDX_PINKY       1
#define IDX_INKY        2
#define IDX_CLYDE       3

/* Speeds of the ghosts and Pacman in cells/second */
#define SPD_PACMAN      3
#define SPD_BLINKY      3.2
#define SPD_PINKY       3
#define SPD_INKY        3
#define SPD_CLYDE       2.8

/* This is the file that all of the following macros apply to */
#define MAP_FILE        "data/pacman.txt"

/* Map dimensions */
#define MAP_WIDTH       84
#define MAP_HEIGHT      33

/* Portal locations */
#define R_PORT_X        27
#define R_PORT_Y        15
#define L_PORT_X        0
#define L_PORT_Y        15

/* There are four locations where ghosts cannot move up */
#define NO_UP_X1        12
#define NO_UP_Y1        12
#define NO_UP_X2        15
#define NO_UP_Y2        12
#define NO_UP_X3        12
#define NO_UP_Y3        25
#define NO_UP_X4        15
#define NO_UP_Y4        25

/* Common values for the ghost house */
#define GHOSTHOUSE_XL   11      /* The X coordinate of left ghost house spot */
#define GHOSTHOUSE_XC   13      /* The X coordinate of center ghost house spot */
#define GHOSTHOUSE_XR   15      /* The X coordinate of right ghost house spot */
#define GHOSTHOUSE_Y    15      /* The Y coordinate of the ghost house */
#define GHOST_EXIT_X    13      /* The X coordinate of first cell outside */
#define GHOST_EXIT_Y    12      /* The Y coordinate of first cell outside */

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

/* Location struct gives location on the map.  Indices will always be >0 */
struct Location {
    int x;
    int y;

    Location() : x(0), y(0) {};
    Location(int x, int y) : x(x), y(y) {};
    bool operator==(Location l) { return x == l.x && y == l.y; }
    bool operator!=(Location l) { return !(*this == l); }
};

/* The four directions, with N being up. */
enum Direction {
    N, E, S, W,
};

/* Calculates the linear distance between two locations */
static inline double linDist(Location l1, Location l2) {
    double x1 = l1.x, x2 = l2.x, y1 = l1.y, y2 = l2.y;
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
};

/* Ghosts can be in one of three states.  This enum defines those. */
enum GhostState {
    RESTART, /* When Pacman dies */
    JAIL,
    SCATTER,
    SCATTER_INT, /* Intermediate value so ghosts know state changed */
    CHASE,
    FRIGHTENED,
    DYING, /* This is when they are on their way to the ghost house */
};

#endif // ifndef COMMON
