#include "game.hpp"

int main(int argc, char **argv) {
    if (argc != 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        exit(1);
    }

    /* Make and run the game */
    Game game;
    game.run();

    return 0;
}
