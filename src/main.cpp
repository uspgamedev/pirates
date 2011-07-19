
#include "game.h"

using pirates::Game;

int main (int argc, char **argv) {
    Game* game = Game::reference();
    game->init(argc, argv);
    game->run();
    delete game;
    return EXIT_SUCCESS;
}

