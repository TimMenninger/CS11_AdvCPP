#ifndef DOCUMENT
#define DOCUMENT

#include "display.hpp"
#include <assert.h>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <math.h>
#include <regex>

/* Line locks will lock segments of this many lines. */
#define LOCK_LINES      10

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

    /********************
    LOCKS
    ********************/
    shared_timed_mutex  fileLock;   /* Locks the entire file */
    shared_timed_mutex  *lineLocks; /* Locks for line groups */
    mutex               upgradeLock;/* This lock is for the rare case in which
                                       we have a read lock on the file, and
                                       need to upgrade to a write lock while
                                       guaranteeing that no other thread edited
                                       anything in the file. We lock the
                                       upgrade lock, un-read-lock the file,
                                       write-lock the file, unlock upgrade. */

    /********************
    PRIVATE HELPERS
    ********************/
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
    vector<Cursor> performSearch(regex);
};

#endif // ifndef DOCUMENT
