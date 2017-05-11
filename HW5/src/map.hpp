#ifndef GAME_MAP
#define GAME_MAP

#include <vector>
#include <assert.h>
#include <fstream>

/* This is what the map contains in each cell. */
typedef char Cell;

class Map {
private:
    /********************
    MEMBERS
    ********************/
    static const unsigned int _width  = 80; /* Width of map */
    static const unsigned int _height = 24; /* Height of map */
    Cell **_cells;                          /* The cells in the map */

    /********************
    PRIVATE FUNCTIONS
    ********************/
    void readFromFile(const char *filename);/* Reads map from argued file */

public:
    /********************
    CONSTRUCTORS
    ********************/
    Map(const char *filename);

    /********************
    DESTRUCTORS
    ********************/
    ~Map();

    /********************
    ACCESSORS
    ********************/
    unsigned int getWidth();
    unsigned int getHeight();
    Cell getCell(unsigned int x, unsigned int y);

    /********************
    MUTATORS
    ********************/
    void setCell(unsigned int x, unsigned int y, Cell c);
};

#endif // ifndef GAME_MAP
