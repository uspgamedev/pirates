
#include "base/game.h"
#include "world/ship.h"

using pirates::base::Game;
using pirates::world::Ship;

static NodePath camera;

int main (int argc, char **argv) {
    Game* game = Game::reference();
    game->Init(argc, argv);
<<<<<<< HEAD
=======
    std::string modelpath("data/king");
    Ship* ship = new Ship(modelpath, game->window(), game->framework());
    ship->taskInitialize(game->taskMgr());
    camera = game->camera();
    camera.set_pos(20, 20, 20);
    camera.look_at(ship->node_pos());
>>>>>>> ab855084771523eb773f7ee9b23379acf003bb34
    game->Run();
    delete game;
    return EXIT_SUCCESS;
}


