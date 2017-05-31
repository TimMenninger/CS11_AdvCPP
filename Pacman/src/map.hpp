#ifndef GAME_MAP
#define GAME_MAP

#include <vector>
#include <assert.h>
#include <fstream>
#include <memory>
#include "common.hpp"

/* The number of characters in a cell */
#define CELL_SIZE       3

/* This is what the map contains in each cell. */
struct Cell;
typedef std::unique_ptr<Cell[]> CellRow;
typedef std::unique_ptr<CellRow[]> CellArray;

enum CellType {
    EMPTY,              /* Contains nothing */
    DOT,                /* Contains an uneaten dot */
    NODOT,              /* Contained a dot that has been eaten */
    POWERUP,            /* Contains an uneaten powerup */
    NOPOWERUP,          /* Contained a powerup tha has been eaten */
    WALL,               /* Contains a wall */
    PORT_R,             /* Port on right side of map, leads to left */
    PORT_L,             /* Port on left side of map, leads to right */
    DOOR,               /* Door to the ghost prison */
};

struct Cell {
    char raw[CELL_SIZE];/* Raw characters the cell is comprised of */
    CellType type;      /* The type of cell */
};

class Map {
private:
    /********************
    MEMBERS
    ********************/
    static const unsigned int _width  = MAP_WIDTH;  /* Width of map */
    static const unsigned int _height = MAP_HEIGHT; /* Height of map */
    CellArray _cells;

    /********************
    PRIVATE FUNCTIONS
    ********************/
    void readFromFile(const char *filename);/* Reads map from argued file */
    CellType decideCellType(char *cell);    /* Gets type of argued cell */

public:
    /********************
    CONSTRUCTORS
    ********************/
    explicit Map(const char *filename);

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
