#ifndef CHANGE
#define CHANGE

#include "document.hpp"

/* This is the interface for changes (insertions and deletions) */
class Change {
private:
    int line;                     /* Line of first character that changed */
    int col;                      /* Column of first character change */
    std::shared_ptr<Document> doc;/* The document changes are in */

protected:
    Change **curr;                /* Current change */
    char diff;                    /* The string that was inserted or deleted */

    /* This function will insert, which is used by children to implement
       the interface. We argue a change because if redo, we aren't taking the
       string from this instance. */
    void doInsert(Change *c) {
        /* Insert the character */
        doc->setCursor(line, col-1);
        doc->insert(diff);
        doc->setCursor(line, col-1);
    };
    /* This function will delete, which is used by children to implement
       the interface. We argue a change because if redo, we aren't taking the
       string from this instance. */
    void doDelete(Change *c) {
        /* Delete the character */
        doc->setCursor(line, col);
        doc->deleteChar();
    };

public:
    Change *next;                 /* Next change in linked list */
    Change *prev;                 /* Previous change in linked list */

    /********************
    CONSTRUCTORS
    ********************/
    Change(std::shared_ptr<Document> d, char s, int l, int c) :
                line(l), col(c), doc(d), diff(s), next(NULL), prev(NULL) {
        curr = (Change **) malloc(sizeof(Change*));
        *curr = this;
    };
    Change(std::shared_ptr<Document> d, char s) : Change(d, s, 0, 0) {};

    /********************
    DESTRUCTORS
    ********************/
    virtual ~Change() {};

    /********************
    MEMBER FUNCTIONS
    ********************/
    void addChange(Change *c) {
        if (c->curr && c->curr != curr)
            delete c->curr;
        c->curr = curr;
        /* If there are changes, delete them */
        if ((*curr)->next != NULL)
            delete ((*curr)->next);
        /* Add to linked list */
        c->prev = *curr;
        (*curr)->next = c;
        *curr = c;
    };

    /********************
    INTERFACE
    ********************/
    virtual void undo() {
        if ((*curr)->diff != '\0')
            (*curr)->undo();
    };
    virtual void redo() {
        if ((*curr)->next != NULL)
            (*curr)->next->redo();
    };
};

/* Insertion class. When undo, should delete. When redo, should insert */
class Insertion : public Change {
public:
    /********************
    CONSTRUCTORS
    ********************/
    Insertion(std::shared_ptr<Document> d, char s, int l, int c) :
        Change(d, s, l, c) {};
    Insertion(std::shared_ptr<Document> d, char s) : Change(d, s) {};

    /********************
    DESTRUCTOR
    ********************/
    ~Insertion() { delete next; };

    /********************
    INTERFACE
    ********************/
    void undo() {
        /* An undo of an insertion is a deletion */
        doDelete(*curr);
        *curr = (*curr)->prev;
    };
    void redo() {
        /* A redo of an insertion is an insertion */
        doInsert((*curr)->next);
        *curr = (*curr)->next;
    };
};

/* Deletion class. When undo, should insert. When redo, should delete */
class Deletion : public Change {
public:
    /********************
    CONSTRUCTORS
    ********************/
    Deletion(std::shared_ptr<Document> d, char s, int l, int c) :
        Change(d, s, l, c) {};
    Deletion(std::shared_ptr<Document> d, char s) : Change(d, s) {};

    /********************
    DESTRUCTOR
    ********************/
    ~Deletion() { delete next; };

    /********************
    INTERFACE
    ********************/
    void undo() {
        /* An undo of an insertion is a deletion */
        doInsert(*curr);
        *curr = (*curr)->prev;
    };
    void redo() {
        /* A redo of an insertion is an insertion */
        doDelete((*curr)->next);
        *curr = (*curr)->next;
    };
};

#endif // ifndef CHANGE
