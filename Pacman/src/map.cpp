#include "map.hpp"

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
    char next[CELL_SIZE];
    std::ifstream f(filename);

    /* Iterate until newline or end of row. */
    int row = 0, col = 0;
    do {
        col = 0; /* Reset column counter */
        do {
            f.get(next[0]); /* Read next character */
            /* Stop if this is the end of the line. */
            if (f.eof())
                return;
            else if (next[0] == '\n')
                break;

            /* Otherwise, get the next two characters to create a Cell */
            for (int i = 1; i < CELL_SIZE; i++)
                f.get(next[i]);

            /* Populate the cell accordingly */
            memcpy((void *) _cells[row][col].raw, (void *) next, CELL_SIZE);
            _cells[row][col].type = decideCellType(next);
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
    _cells = CellArray(new CellRow[_height]);

    /* Defines an empty cell */
    Cell emptyCell;
    memset((void *) emptyCell.raw, ' ', CELL_SIZE);

    for (int y = 0; y < _height; y++) {
        /* Allocate memory and check pointer validity. */
        _cells[y] = std::unique_ptr<Cell[]>(new Cell[_width]);

        /* Fill this row with all empty cells. */
        for (int x = 0; x < _width; x++)
            _cells[y][x] = emptyCell;
    }

    /* Read the map from the file */
    readFromFile(filename);
}

/*
 ~Map

 Destructor of map class.

 Arguments: None.

 Returns:   None.
*/
Map::~Map() {
    /* Used smart pointers */
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

/*
 decideCellType

 Takes a set of characters that defines a cell in the map and returns the
 CellType that describes it.  Note that this will return EMPTY for any set of
 three spaces, it will not ever return that a dot has been eaten.

 Arguments: char *cell - the characters that make up the cell

 Returns:   CellType - the type of cell argued
*/
CellType Map::decideCellType(char *cell) {
    /* Decide the cell type by checking all possibilities (made best attempt
       to check in decreasing order of frequency) */
    if (!memcmp((void *) cell, " . ", CELL_SIZE))
        return DOT;
    if (!memcmp((void *) cell, "   ", CELL_SIZE))
        return EMPTY;
    if (!memcmp((void *) cell, " O ", CELL_SIZE))
        return POWERUP;
    if (!memcmp((void *) cell, "___", CELL_SIZE))
        return DOOR;
    if (!memcmp((void *) cell, "<  ", CELL_SIZE))
        return PORT_R;
    if (!memcmp((void *) cell, "  >", CELL_SIZE))
        return PORT_L;
    return WALL;
}
