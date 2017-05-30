#include "vim.hpp"

/*
 Vim

 Constructors

 Arguments: string filename - the name of the file we are editing

 Returns:   None.
*/
Vim::Vim(string filename) {
    /* Start in default state */
    _state = DEFAULT;

    /* Set up document */
    _filename = filename;
    _doc = std::shared_ptr<Document>(new Document(filename));

    /* Set up display */
    _disp = std::shared_ptr<Display>(new Display);
    _textWindow  = _disp->createWindow(0, 0, COLS, LINES-1);
    _stateWindow = _disp->createWindow(0, LINES-1, COLS, LINES);

    /* Move the cursor to the top left of the window and file */
    setCursor(0, 0);
    line0 = 0;
    col0 = 0;

    /* Display file */
    outputFileToDisplay();

    /* Display file contents at bottom of screen */
    outputFileInfo();

    /* Default do each action once. */
    _times = -1;

    /* Put a dummy change in to mark the beginning of the list. */
    changes = new Change(_doc, '\0');

    refresh();
}

/*
 ~Vim

 Destructor

 Arguments: None.

 Returns:   None.
*/
Vim::~Vim() {}

/*
 outputFileToDisplay

 Takes the file stored in the Document and prints everything that it can
 to the display.

 Arguments: None.

 Returns:   None.
*/
void Vim::outputFileToDisplay() {
    /* Get the cursor location so we don't edit it at all. */
    Cursor curs = _doc->getCursor();

    /* Print only the lines that will fit. */
    for (int i = line0; i < line0 + LINES - 1; i++) {
        _disp->putLine(_textWindow, i-line0, (char *) _doc->getLine(i).c_str() + col0, false);
    }

    /* Restore the cursor location */
    setCursor(curs.line, curs.col, true);

    /* Refresh the window */
    _disp->refreshWindow(_textWindow);
}

/*
 outputFileInfo

 Fills the bottom line of the terminal with the filename, the number of lines
 and the number of characters.

 Arguments: None.

 Returns:   None.
*/
void Vim::outputFileInfo() {
    /* Get the cursor location so we don't edit it at all. */
    Cursor curs = _doc->getCursor();

    /* Get the number of lines and characters. */
    int nLines, nChars;
    _doc->getDimensions(nLines, nChars);

    /* Output the line at the bottom */
    char *line = (char *) string("\"" + _filename + "\" " + \
                                 to_string(nLines) + "L, " + \
                                 to_string(nChars) + "C").c_str();
    _disp->putLine(_stateWindow, 0, line, true);

    /* Restore the cursor location */
    setCursor(curs.line, curs.col, true);
}

/*
 outputStateToDisplay

 Fills the bottom line of the terminal with the string appropriate for the
 current state.

 Arguments: None.

 Returns:   None.
*/
void Vim::outputStateToDisplay() {
    /* Get the cursor location so we don't edit it at all. */
    Cursor curs = _doc->getCursor();

    /* Do this based on the state */
    switch (_state) {
    case INSERT:
        _disp->putLine(_stateWindow, 0, (char *) string("-- INSERT --").c_str(), true);
        break;
    case REPLACE:
        _disp->putLine(_stateWindow, 0, (char *) string("-- REPLACE --").c_str(), true);
        break;
    case DEFAULT:
        /* Follow through */
    default:
        _disp->putLine(_stateWindow, 0, (char *) string("").c_str(), true);
        break;
    }

    /* Restore the cursor location */
    setCursor(curs.line, curs.col, true);
}

/*
 setCursor

 Sets the cursor in both the document and the terminal screen.

 Arguments: int line - Line number to move to
            int col - Column number to move to
            bool incNL - if true, this can put the cursor at the newline
                spot. Default is false.

 Returns:   None.
*/
void Vim::setCursor(int line, int col, bool incNL) {
    /* Set cursor in document. This will limit the location to the size
       of the file/line the cursor is in and returns the acutal location
       of the cursor. */
    Cursor c = _doc->setCursor(line, col, incNL);

    /* Check if we must scroll. Keep a flag so we only rewrite file when
       necessary. */
    bool scrolled = false;
    if ((scrolled |= c.line < line0))
        line0 = c.line;
    else if ((scrolled |= c.line >= line0 + LINES - 1))
        line0 = c.line - LINES + 2;
    if (c.col < col0) {
        scrolled = true;
        col0 = c.col;
    }
    else if (c.col >= col0 + COLS) {
        scrolled = true;
        col0 = c.col - COLS + 1;
    }

    if (scrolled)
        outputFileToDisplay();

    /* Set the cursor in the terminal */
    wmove(_textWindow, c.line-line0, c.col-col0);
    wrefresh(_textWindow);
}

/*
 run

 Runs the Vim emulator.

 Arguments: None.

 Returns:   None.
*/
void Vim::run() {
    while (1) {
        /* Get the next character, then use the state machine to decide what
           to do with it. */
        int c = getch();

        /* Most of these will use the cursor location */
        Cursor curs = _doc->getCursor();

        /* Our simple state machine */
        switch (_state) {
        case DEFAULT:
            /************************************
            ARROW KEYS and ARROW CHARS
            ************************************/
            if (c == KEY_UP || c == 'k')
                setCursor(curs.line -= abs(_times), curs.col);
            else if (c == KEY_DOWN || c == 'j')
                setCursor(curs.line += abs(_times), curs.col);
            else if (c == KEY_RIGHT || c == 'l')
                setCursor(curs.line, curs.col += abs(_times));
            else if (c == KEY_LEFT || c == 'h')
                setCursor(curs.line, curs.col -= abs(_times));
            /************************************
            MOVE TO (0, 0)
            ************************************/
            else if (c == 'H')
                setCursor(0, 0);
            /************************************
            MOVE TO END OF LINE
            ************************************/
            else if (c == '$')
                setCursor(curs.line + (abs(_times)-1), INT_MAX);
            /************************************
            MOVE TO BEGINNING OF LINE
            ************************************/
            else if (c == '^')
                setCursor(curs.line, 0);
            /************************************
            SAVE THE FILE
            ************************************/
            else if (c == 'W') {
                _doc->save(_filename);
                outputFileInfo();
            }
            /************************************
            QUIT WITHOUT SAVING
            ************************************/
            else if (c == 'Q')
                return;
            /************************************
            DELETE CHARACTER
            ************************************/
            else if (c == 'x') {
                _times = abs(_times);
                curs.col++; /* Increment here so we delete char before */
                while (_times-- > 0) {
                    /* Temporary cursor for change list integrity */
                    Cursor newCurs = _doc->setCursor(curs.line, curs.col, true);
                    changes->addChange(new Deletion
                        (_doc, _doc->deleteChar(), curs.line, curs.col));
                    curs = newCurs;
                    if (curs.col == 0)
                        /* At this point, we've deleted entire line */
                        break;

                    outputFileToDisplay();
                }
            }
            /************************************
            INCREASE MULTIPLIER
            ************************************/
            else if (c >= '0' && c <= '9') {
                if (_times == -1)
                    _times = c == '0' ? -1 : c - '0';
                else
                    _times = 10*_times + (c-'0');
            }
            /************************************
            INSERT MODE
            ************************************/
            else if (c == 'i') {
                _state = INSERT;
                outputStateToDisplay();
            }
            else if (c == 'a') {
                setCursor(curs.line, curs.col+1, true);
                _state = INSERT;
                outputStateToDisplay();
            }
            /************************************
            REPLACE MODE
            ************************************/
            else if (c == 'R') {
                _state = REPLACE;
                outputStateToDisplay();
            }
            /************************************
            UNDO
            ************************************/
            else if (c == 'u') {
                changes->undo();
                outputFileToDisplay();
            }
            /************************************
            REDO
            ************************************/
            else if (c == '.') {
                changes->redo();
                outputFileToDisplay();
            }
            /************************************
            TRAVERSE WORDS
            ************************************/
            else if (c == 'w') {
                /* Next word */
                curs = _doc->nextWord();
                setCursor(curs.line, curs.col);
            }
            else if (c == 'b') {
                /* Previous word */
                curs = _doc->prevWord();
                setCursor(curs.line, curs.col);
            }
            else if (c == 'e') {
                /* End of word */
                curs = _doc->endOfWord();
                setCursor(curs.line, curs.col);
            }
            break;
        case REPLACE:
            /************************************
            BACKSPACE
            ************************************/
            if (c == 127)
                setCursor(curs.line, --curs.col);
            /************************************
            ARROW KEYS and ARROW CHARS
            ************************************/
            else if (c == KEY_UP)
                setCursor(--curs.line, curs.col);
            else if (c == KEY_DOWN)
                setCursor(++curs.line, curs.col);
            else if (c == KEY_RIGHT)
                setCursor(curs.line, ++curs.col);
            else if (c == KEY_LEFT)
                setCursor(curs.line, --curs.col);
            /************************************
            REPLACE CHARACTER
            ************************************/
            else if ((int) c >= 32) {
                _doc->setCursor(curs.line, curs.col+1, true);
                /* Do and record the deletion */
                changes->addChange(new Deletion
                    (_doc, _doc->deleteChar(), curs.line, curs.col+1));
                /* Do and record the insertion */
                changes->addChange(new Insertion
                    (_doc, c, curs.line, curs.col+1));
                _doc->insert(c);
                outputFileToDisplay();
            }
            /************************************
            INSERT NEWLINE
            ************************************/
            else if (c == 10) {
                _doc->insert('\n');
                outputFileToDisplay();
            }
            /************************************
            EXIT REPLACE MODE
            ************************************/
            else if (c == 27)  {
                _state = DEFAULT;
                setCursor(curs.line, curs.col-1);
                outputStateToDisplay();
            }
            break;
        case SEARCH:
            break;
        case INSERT:
            /************************************
            BACKSPACE
            ************************************/
            if (c == 127) {
                _doc->deleteChar();
                outputFileToDisplay();
            }
            /************************************
            ARROW KEYS and ARROW CHARS
            ************************************/
            else if (c == KEY_UP)
                setCursor(--curs.line, curs.col, true);
            else if (c == KEY_DOWN)
                setCursor(++curs.line, curs.col, true);
            else if (c == KEY_RIGHT)
                setCursor(curs.line, ++curs.col, true);
            else if (c == KEY_LEFT)
                setCursor(curs.line, --curs.col);
            /************************************
            INSERT CHARACTER
            ************************************/
            else if ((int) c >= 32) {
                /* Do and record the insertion */
                changes->addChange(new Insertion
                    (_doc, c, curs.line, curs.col+1));
                _doc->insert(c);
                outputFileToDisplay();
            }
            /************************************
            INSERT NEWLINE
            ************************************/
            else if (c == 10) {
                /* Do and record the insertion */
                changes->addChange(new Insertion
                    (_doc, c, curs.line, curs.col+1));
                _doc->insert('\n');
                outputFileToDisplay();
            }
            /************************************
            EXIT INSERT MODE
            ************************************/
            else if (c == 27)  {
                _state = DEFAULT;
                /* This will put the cursor off the newline if applicable */
                setCursor(curs.line, curs.col);
                outputStateToDisplay();
            }
            break;
        default:
            return;
        }

        /* Reset the counter for action times unless this was a character that
           actively changed it. */
        if (_state != DEFAULT || c < '0' || c > '9')
            _times = -1;
    }
}
