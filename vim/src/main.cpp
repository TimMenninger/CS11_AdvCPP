#include <stdlib.h>
#include <iostream>
#include "vim.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>", argv[0]);
        exit(1);
    }

    /* Make and run the game */
    Vim v(argv[1]);
    v.run();

    return 0;
}
