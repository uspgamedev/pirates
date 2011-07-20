
#include "game.h"

using pirates::Game;

int main (int argc, char **argv) {
    Game* game = Game::reference();
    game->Init(argc, argv);
    game->Run();
    delete game;
    return EXIT_SUCCESS;
}

