#include "document.hpp"

/*
 Document

 Constructor.

 Arguments: char filename - the file to open

 Returns:   None.
*/
Document::Document() {}
Document::Document(string fn) {
    /* Originally, there have been no changes to the file. */
    dirty = false;

    /* Populate the file if applicable. */
    importFile(fn);
}

/*
 ~Document

 Destructor.

 Arguments: None.

 Returns:   None.
*/
Document::~Document() {
    /* Nothing here when using smart pointers */
}

/*
 importFile

 Reads the file, if it exists, and populates the internal file char arrays
 with them.

 Arguments: None.

 Returns:   None.
*/
void Document::importFile(string filename) {
    /* Open the file for reading. */
    fstream f(filename);

    /* If it exists, then use it to populate our vectors. */
    if (f.good()) {
        /* Get a line */
        string line;

        while (!f.eof()) {
            /* Read the line */
            getline(f, line);

            /* Put the line into the array of lines. We add a newline because
               apparently that's dropped by getline() */
            line.push_back('\n');
            file.push_back(line);
        }

        /* Because of while condition, we have one extra line */
        file.pop_back();
    }

    /* Make sure to close the file. */
    f.close();
}

/*
 setCursor

 Sets the cursor on the internal file.

 Arguments: int line - the line number for the cursor
            int col - the column number for the cursor
            bool incNL - if true, this can put the cursor at the newline
                spot. Default is false.

 Returns:   Cursor - the actual location of the cursor.
*/
Cursor Document::setCursor(int line, int col, bool incNL) {
    /* Because we use line to index, we need to make sure it is within the
       bounds of our file. Also check here that column is greater than zero
       just because it has to happen somewhere... */
    line = min(max(line, 0), ((int) file.size())-1);
    col  = max(col, 0);

    /* Set the cursor */
    cursor = Cursor(line, col);

    /* Put the cursor in bounds. If the line has only a newline, we can put
       the character there. We do line size -2: -1 for 0-indexing and
       another -1 for the newline */
    cursor.line = min(cursor.line, ((int) file.size())-1);
    cursor.col  = max(0, min(cursor.col, ((int) file[line].size())-(2-incNL)));

    return cursor;
}

/*
 getCursor

 Returns the cursor location.

 Arguments: None.

 Returns:   Cursor - the cursor location int he file
*/
Cursor Document::getCursor() {
    return cursor;
}

/*
 insert

 Inserts a character into the file to the left of the cursor location.

 Arguments: char c - the character to insert

 Returns:   None.
*/
void Document::insert(char c) {
    /* The file is now changed/dirty */
    dirty = true;

    /* Insert the character, moving the cursor right one */
    file[cursor.line].insert(file[cursor.line].begin() + cursor.col++, c);

    /* If the character is a newline, we must create a new line with
       the current character and everything to its right. */
    if (c == '\n' || c == '\r') {
        /* Everything to the right of the cursor including itself. We
           increment the cursor line number to the place where we want to
           put the line right before. */
        string newline = file[cursor.line].substr(cursor.col);
        file[cursor.line++].erase(cursor.col);

        /* Put it in the new line. The line number is now in the right spot,
           but we want the column to be at the left of the line now. */
        file.insert(file.begin() + cursor.line, newline);
        cursor.col = 0;
    }
}

/*
 deleteChar

 Deletes a character from the file to the left of the cursor location.  This
 moves the cursor back one. If the cursor is at the beginning of the line and
 there is a line above it, it deletes the newline from the line above and
 the current line becomes a part of that one.

 Arguments: None.

 Returns:   char - the deleted character.
*/
char Document::deleteChar() {
    /* If the column is 0, i.e. we're at the left of a line, first append
       this line to the one above if there is one. We will then go on to
       delete normally, which will remove the newline that separates the
       two lines */
    if (cursor.col == 0 && cursor.line > 0) {
        /* Line is now one above. */
        cursor.line--;

        /* Column remains first character here, which is just past the last
           character fo the previous line, which cursor.line is now at after
           decrementing before. */
        cursor.col = file[cursor.line].size();

        /* Concatenate the two strings into this one. */
        file[cursor.line].insert(cursor.col, file[cursor.line+1]);

        /* Remove the original line that was moved to the line above. */
        file.erase(file.begin() + cursor.line + 1);
    }

    /* Delete a character (if we were at the beginning of a line, we will
       erase the newline here). If we are at the beginning of a line, we
       are at the beginning of the file and won't delete anything. */
    char deleted = '\0';
    if (file[cursor.line].size() == 1)
        cursor.col--;
    else if (cursor.col != 0) {
        /* The file is now dirty */
        dirty = true;
        /* Delete the character */
        deleted = file[cursor.line].at(cursor.col-1);
        file[cursor.line].erase(file[cursor.line].begin() + --cursor.col);
    }

    return deleted;
}

/*
 save

 Saves the file.

 Arguments: string toSaveTo - the name of the file to save to

 Returns:   None.
*/
void Document::save(string toSaveTo) {
    /* Open the file for writing. */
    ofstream f(toSaveTo);

    /* Write each line, which should include newlines for us. */
    int currLine = -1;
    while (++currLine < file.size())
        f.write(file[currLine].c_str(), file[currLine].size());

    f.close();
}

/*
 getLine

 Returns a line of the file at the argued index.  If the line is out of the
 index range, an empty string is returned.

 Arguments: int line - the line number to return

 Returns:   string - the line at the argued index.
*/
string Document::getLine(int line) {
    /* Return empty string if the line is out of range */
    if (line < 0 || line >= file.size())
        return string("");
    return file[line];
}

/*
 isDirty

 Returns whether the dirty flag is set.

 Arguments: None.

 Returns:   bool - reflects dirty flag
*/
bool Document::isDirty() {
    return dirty;
}

/*
 setDirty

 Sets the dirty flag to the argued value.

 Arguments: bool d - what to set the dirty flag to

 Returns:   None.
*/
void Document::setDirty(bool d) {
    dirty = d;
}

/*
 getDimensions

 Returns the number of lines and characters in the file.

 Arguments: int &nLines - reference to where to store number of lines
            int &nChars - reference to where to store number of characters

 Returns:   None.
*/
void Document::getDimensions(int &nLines, int &nChars) {
    /* Zero out the characters argument */
    nLines = file.size();
    nChars = 0;

    /* Count */
    for (int i = 0; i < nLines; i++)
        nChars += file[i].size();
}

/*
 nextWord

 Returns the cursor of the first character of the next word of the file, going
 to the next line if necessary. If already on the beginning of a word, it goes
 to the next word. If at the end of the file, it goes to the last character.
 If a line is empty except for a newline, that newline will be returned.

 Arguments: None.

 Returns:   Cursor - location of the next word.
*/
Cursor Document::nextWord() {
    /* Flag tells us if we've found whitespace, which means the next character
       that isn't whitespace will be what's returned. */
    bool whitespace = false;

    int l = cursor.line, c = cursor.col;
    /* If we are on a line with just a newline, we will start one line lower
       to kickstart movement */
    if (file[l].size() == 1)
        l = (l + 1 == file.size()) ? l : l + 1;

    while (!whitespace) {
        /* See if we found whitespace */
        whitespace = file[l].at(c) == ' ' || file[l].at(c) == '\n';
        c++;
    }
    /* Undo the last increment */
    c--;

    /* Iterate until not whitespace */
    while (whitespace) {
        whitespace = file[l].at(c) == ' ' || file[l].at(c) == '\n';

        /* Wrap if we are at a new line */
        if (file[l].at(c++) == '\n') {
            /* Unless this is the only character in the line */
            if (file[l].size() == 1)
                whitespace = false;
            else {
                c = 0;
                l++;
            }
        }

        /* If we are at end of file, we also found it */
        if (l >= file.size()) {
            l = file.size() - 1;
            c = file[l].size() - 1;
            whitespace = false;
        }
    }

    /* Next character is the ticket, unless we are already at last character
       of file or a line with only a newline */
    if (file[l].size() == 1)
        return Cursor(l, 0);
    if (l + 1 == file.size() && c + 1 == file[l].size())
        return Cursor(l, c);
    if (c + 1 == file[l].size())
        return Cursor(++l, 0);
    return Cursor(l, --c);
}

/*
 prevWord

 Returns the cursor of the first character of the previous word of the file,
 going to the previous line if necessary. If already on the beginning of a word,
 it goes to the previous word.

 Arguments: None.

 Returns:   Cursor - location of the next word.

 Bugs:      When in the middle of the word, this takes you to the beginning of
            the previous word instead of the beginning of the current word.
*/
Cursor Document::prevWord() {
    int c = cursor.col, l = cursor.line;

    /* Kickstart movement if we are at beginning of line already */
    if (c == 0 && l > 0) {
        l--;
        c = file[l].size() - 1;
    }
    else if (c == 0)
        return Cursor(0, 0);

    int idx = c;
    /* If we are on a character, go until the beginning of this word or the
       beginning of the line, whichever comes first */
    while (file[l].at(idx) != ' ' && file[l].at(idx) != '\n' && idx > 0)
        idx--;

    /* If idx and c are different, we are at the beginning of the word */
    if (idx - c > 1)
        return Cursor(l, idx);
    c = idx;

    /* Otherwise, go backwards until we are not at whitespace */
    while (file[l].at(c) == ' ' || file[l].at(c) == '\n') {
        /* If we're at the beginning of the line, move up a line */
        if (c == 0 && l > 0) {
            l--;
            c = file[l].size() - 1;
        }
        else if (c == 0 && l == 0)
            return Cursor(0, 0);

        /* Decrement column counter */
        c--;
    }

    /* Now iterate to beginning of word */
    while (c >= 0 && file[l].at(c) != ' ' && file[l].at(c) != '\n')
        c--;

    return Cursor(l, ++c);
}

/*
 endOfWord

 Returns the cursor on the last character of the current or next word.

 Arguments: None.

 Returns:   Cursor - location of end of word.
*/
Cursor Document::endOfWord() {
    int l = cursor.line, c = cursor.col;

    /* If not on a character, go to beginning of next word */
    if (file[l].at(c) == ' ' || file[l].at(c) == '\n') {
        Cursor curs = nextWord();
        l = curs.line;
        c = curs.col;
    }
    else if (c < file[l].size() - 1 &&
            (file[l].at(c+1) == ' ' || file[l].at(c+1) == '\n')) {

        Cursor curs = nextWord();
        l = curs.line;
        c = curs.col;
    }

    /* If this is end of file, we are done. */
    if (l == file.size() - 1 && c == file[l].size())
        return Cursor(l, c);

    /* Iterate until whitespace */
    while (file[l].at(c) != ' ' && file[l].at(c) != '\n')
        c++;

    return Cursor(l, --c);
}
