#ifndef COMMON
#define COMMON

#include <math.h>

/* Location struct gives location on the map.  Indices will always be >0 */
struct Location {
    int x;
    int y;

    Location() : x(0), y(0) {};
    Location(int x, int y) : x(x), y(y) {};
};

/* The four directions, with N being up. */
enum Direction {
    N, E, S, W,
};

/* Calculates the linear distance between two locations */
static inline double linDist(Location l1, Location l2) {
    return sqrt((l2.x - l1.x) * (l2.x - l1.x) + (l2.y - l1.y) * (l1.y - l1.y));
};

#endif // ifndef COMMON
