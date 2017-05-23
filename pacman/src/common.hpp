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
    double x1 = l1.x, x2 = l2.x, y1 = l1.y, y2 = l2.y;
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
};

#endif // ifndef COMMON
