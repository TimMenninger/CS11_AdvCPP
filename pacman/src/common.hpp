#ifndef COMMON
#define COMMON

#include <math.h>

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
