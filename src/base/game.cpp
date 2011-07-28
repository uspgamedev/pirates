
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
    load_prc_file_data("", "model-path .");
    InputManager::reference()->Init();
    camera_node_ = window_->get_camera_group();
    window_->setup_trackball();
    Ship* ship = new Ship();
    ship->taskInitialize(taskMgr());
    camera_node_.set_pos(0, 0, 20);
    camera_node_.look_at(LPoint3f(0,0,0));
}

} // namespace base
} // namespace pirates
