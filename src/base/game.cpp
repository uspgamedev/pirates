
#include "base/game.h"
#include "base/inputmanager.h"
#include "world/ship.h"
#include "load_prc_file.h"

namespace pirates {

using world::Ship;

namespace base {

Game* Game::reference_ = NULL;

void Game::Init (int &argc, char **&argv) {
    framework_.open_framework(argc, argv);
    framework_.set_window_title("Pirates by USPGameDev");
    window_ = framework_.open_window();
    InputManager::reference()->Init();
    camera_node_ = window_->get_camera_group();
    load_prc_file_data("", "model-path .");
    Ship* ship = new Ship();
    ship->taskInicialize(taskMgr());
    camera_node_.set_pos(20, 20, 20);
    camera_node_.look_at(ship->node_pos());
}

} // namespace base
} // namespace pirates
