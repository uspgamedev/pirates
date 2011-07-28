
#include "base/game.h"
#include "world/ship.h"

using pirates::base::Game;
using pirates::world::Ship;

static NodePath camera;

int main (int argc, char **argv) {
    Game* game = Game::reference();
    game->Init(argc, argv);
    game->Run();
    delete game;
    return EXIT_SUCCESS;
}


