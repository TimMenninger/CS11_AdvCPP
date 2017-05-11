#include "map.hpp"
#include <iostream>
/*
 readFromFile (private)

 Reads map from a file and populates the map vector.

 Arguments: const char *filename - name of the file to read from

 Returns:   None.
*/
void Map::readFromFile(const char *filename) {
    /* Ensure that the argument is a valid pointer. */
    assert(filename);

    /* The object used to input values.  We will go until the end of the
       line or the width of the map, whichever comes first.  If the former,
       then the rest of the row will be filled with blank spaces. */
    char next;
    std::ifstream f(filename);

    /* Iterate until newline or end of row. */
    int row = 0, col = 0;
    do {
        col = 0; /* Reset column counter */
        do {
            f.get(next); /* Read next character */
            /* Stop if this is the end of the line. */
            if (f.eof())
                return;
            else if (next == '\n')
                break;
            else
                _cells[row][col] = next;
        } while (col++ < _width);
    } while (row++ < _height);
}

/*
 Map

 Constructor, takes the filename to populate the map from.

 Arguments: const char *filename - name of the file to read from

 Returns:   None.
*/
Map::Map(const char *filename) {
    /* Fills the Cell array with empty cells, which are spaces. */
    _cells = (Cell **) malloc(_height * sizeof(Cell *));
    if (!_cells) {
        fprintf(stderr, "out of memory");
        exit(1);
    }
    for (int y = 0; y < _height; y++) {
        /* Allocate memory and check pointer validity. */
        _cells[y] = (Cell *) malloc(_width * sizeof(Cell));
        if (!_cells[y]) {
            fprintf(stderr, "out of memory");
            exit(1);
        }

        /* Fill this row with all spaces. */
        memset((void *) _cells[y], ' ', _width * sizeof(Cell));
    }

    /* Read the map from the file */
    readFromFile(filename);
}

/*
 ~Map

 Destructor of rmap class.

 Arguments: None.

 Returns:   None.
*/
Map::~Map() {
    /* Destroy the cells array. */
    for (int y = 0; y < _height && _cells; y++)
        if (_cells[y]) delete _cells[y];
    if (_cells) delete _cells;
}

/*
 getWidth

 Gets the width of the map.

 Arguments: None.

 Returns:   unsigned int - the width of the map
*/
unsigned int Map::getWidth() { return _width; }

/*
 getHeight

 Gets the height of the map.

 Arguments: None.

 Returns:   unsigned int - the height of the map
*/
unsigned int Map::getHeight() { return _height; }

/*
 getCell

 Returns the value in the argued cell.

 Arguments: int x - x coordinate of desired cell
            int y - y coordinate of desired cell

 Returns:   Cell - the cell at (x, y)
*/
Cell Map::getCell(unsigned int x, unsigned int y) {
    assert(x < getWidth() && y < getHeight());
    return _cells[y][x];
}

/*
 setCell

 Sets the value of a cell in the map.

 Arguments: int x - x coordinate of desired cell
            int y - y coordinate of desired cell
            Cell c - new value of cell

 Returns:   None.
*/
void Map::setCell(unsigned int x, unsigned int y, Cell c) {
    assert(x < getWidth() && y < getHeight());
    _cells[y][x] = c;
}
