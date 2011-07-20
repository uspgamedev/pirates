
#include "game.h"
#include "inputmanager.h"

namespace pirates {

Game* Game::reference_ = NULL;

void Game::Init (int &argc, char **&argv) {
    framework_.open_framework(argc, argv);
    framework_.set_window_title("Pirates by USPGameDev");
    window_ = framework_.open_window();
    InputManager::reference()->Init();
}

}

