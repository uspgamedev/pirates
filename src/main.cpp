
#include "base/game.h"
#include "base/boat.h"

using pirates::base::Game;
using pirates::base::Boat;

static NodePath camera;

int main (int argc, char **argv) {
    Game* game = Game::reference();
    game->Init(argc, argv);
    std::string modelpath("panda-model");
    Boat* boat = new Boat( modelpath, game->window(), game->framework());
    boat->taskInicialize( game->taskMgr() );
    camera = game->camera();
    camera.set_pos(20, 20, 20);
    camera.look_at(boat->node_pos());
    game->Run();
    delete game;
    return EXIT_SUCCESS;

}


