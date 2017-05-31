#include <sys/ioctl.h>
#include <iostream>
#include "game.hpp"
#include "common.hpp"

int main(int argc, char **argv) {
    if (argc != 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        exit(1);
    }

    /* Get original terminal size */
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    size_t origRows = size.ws_row, origCols = size.ws_col;

    /* Make sure terminal is large enough */
    size_t rows = origRows, cols = origCols;
    while (rows != MAP_HEIGHT+4 || cols != MAP_WIDTH) {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        rows = size.ws_row;
        cols = size.ws_col;
        std::cout << "\e[8;" << MAP_HEIGHT+4 << ";" << MAP_WIDTH << "t";
    }

    /* Make and run the game */
    Game game;
    game.run();

    /* Restore terminal size */
    std::cout << "\e[8;" << origRows << ";" << origCols << "t";

    return 0;
}
