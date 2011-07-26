
#include "game.h"
#include "boat.h"
#include <string>

using pirates::Game;
using pirates::Boat;

static NodePath camera;

int main (int argc, char **argv) {
    Game* game = Game::reference();
    game->init(argc, argv);
    std::string modelpath("panda-model");
    Boat* boat = new Boat( modelpath, game->window(), game->framework());
    boat->taskInicialize( game->taskMgr() );
    camera = game->camera();
    camera.set_pos(20, 20, 20);
    camera.look_at(boat->node_pos());
    game->run();
    delete game;
    return EXIT_SUCCESS;
}

