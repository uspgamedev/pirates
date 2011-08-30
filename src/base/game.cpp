
#include "base/game.h"
#include "base/inputmanager.h"
#include "world/ship.h"
#include "world/planet.h"
#include "load_prc_file.h"

namespace pirates {

using world::Ship;
using world::Planet;

namespace base {

Game* Game::reference_ = NULL;

void Game::Init (int &argc, char **&argv) {
    framework_.open_framework(argc, argv);
    framework_.set_window_title("Pirates by USPGameDev");
    window_ = framework_.open_window();
    load_prc_file_data("", "model-path .");
    camera_node_ = window_->get_camera_group();
    camera_node_.set_pos(60,-120,150);
    camera_node_.look_at(0,0,0);

    ship_ = new Ship();
    ship_->taskInitialize(taskMgr());

    Planet* planet = new Planet();
    InputManager::reference()->Init();
}

} // namespace base
} // namespace pirates
