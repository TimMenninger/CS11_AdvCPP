#ifndef DOCUMENT
#define DOCUMENT

#include "display.hpp"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

/* The cursor has a line and a column. */
struct Cursor {
    int line;
    int col;
    Cursor() : line(0), col(0) {};
    Cursor(int l, int c) : line(l), col(c) {};
};

/* The document class handles interaction with the file system as well as
   with the display (ncurses) */
class Document {
private:
    vector<string>  file;       /* The contents of the file */
    Cursor          cursor;     /* The location of the cursor. */
    bool            dirty;      /* Keeps track of if changes have been made. */

    void importFile(string);

public:
    /********************
    CONSTRUCTORS
    ********************/
    Document();
    Document(string filename);

    /********************
    DESTRUCTORS
    ********************/
    ~Document();

    /********************
    MEMBERS
    ********************/
    Cursor setCursor(int, int, bool=false);
    Cursor getCursor();
    void insert(char);
    char deleteChar();
    void save(string);
    string getLine(int);
    void getDimensions(int&, int&);
    bool isDirty();
    void setDirty(bool);
    Cursor nextWord();
    Cursor prevWord();
    Cursor endOfWord();
};

#endif // ifndef DOCUMENT
