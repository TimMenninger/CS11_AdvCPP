#include "game.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <path_to_map>", argv[0]);
        exit(1);
    }

    /* Make and run the game */
    Game game(argv[1]);
    game.run();

    return 0;
}
