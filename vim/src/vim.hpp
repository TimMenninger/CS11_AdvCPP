#ifndef VIM
#define VIM

#include "document.hpp"
#include "display.hpp"
#include "change.hpp"
#include <unistd.h>
#include <thread>
#include <vector>
#include <regex>

/* Number of seconds to wait before auto saving */
#define AUTO_SAVE_SECS  30

/* The enumeration of possible states for the vim editor */
enum VimState {
    DEFAULT,
    SEARCH,
    REPLACE,
    INSERT,
};

class Vim {
private:
    std::shared_ptr<Document> _doc;     /* The document vim is editing */
    std::shared_ptr<Display>  _disp;    /* The display vim is on */
    WindowSPtr _textWindow;             /* Where text is displayed */
    WindowSPtr _stateWindow;            /* Where the status is put */
    string _filename;                   /* The file we are editing */
    string _tempFilename;               /* Name of temp autosaved file */
    string _lastSearch;                 /* Last string that was searched */
    char _searchDir;                    /* Search forward '/' or backward '?' */
    VimState _state;                    /* The editing state */
    int _times;                         /* Times to perform next action */
    int line0;                          /* Index of first line displayed */
    int col0;                           /* Index of first char displayed */
    Change *changes;                    /* Linked list of changes */
    bool done;                          /* When true, we should stop Vim */
    vector<Cursor> searchResults;       /* Locations of search results */

    std::string generateTempFilename();
    void outputFileToDisplay();
    void outputFileInfo();
    void outputStateToDisplay();
    void setCursor(int, int, bool=false);
    void keyListener();
    void autoRecovery();
    Cursor nextSearchResult(Cursor);

public:
    /********************
    CONSTRUCTORS
    ********************/
    explicit Vim(string filename);

    /********************
    DESTRUCTORS
    ********************/
    ~Vim();

    /********************
    MEMBERS
    ********************/
    void run();
};

#endif // ifndef VIM
