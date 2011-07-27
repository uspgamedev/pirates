
#include "base/game.h"
#include "world/ship.h"

using pirates::base::Game;
using pirates::world::Ship;

static NodePath camera;

int main (int argc, char **argv) {
    Game* game = Game::reference();
    game->Init(argc, argv);
    std::string modelpath("data/king");
    Ship* ship = new Ship(modelpath, game->window(), game->framework());
    ship->taskInicialize(game->taskMgr());
    camera = game->camera();
    camera.set_pos(20, 20, 20);
    camera.look_at(ship->node_pos());
    game->Run();
    delete game;
    return EXIT_SUCCESS;

}


