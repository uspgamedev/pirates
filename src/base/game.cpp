
#include "base/game.h"
#include "base/inputmanager.h"
#include "load_prc_file.h"

namespace pirates {
namespace base {

Game* Game::reference_ = NULL;

void Game::Init (int &argc, char **&argv) {
    framework_.open_framework(argc, argv);
    framework_.set_window_title("Pirates by USPGameDev");
    window_ = framework_.open_window();
    InputManager::reference()->Init();
    camera_node_ = window_->get_camera_group();
    load_prc_file_data("", "model-path .");
}

} // namespace base
} // namespace pirates
