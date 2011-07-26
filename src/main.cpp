
#include "base/game.h"

using pirates::base::Game;

int main (int argc, char **argv) {
    Game* game = Game::reference();
    game->Init(argc, argv);
    game->Run();
    delete game;
    return EXIT_SUCCESS;

}

