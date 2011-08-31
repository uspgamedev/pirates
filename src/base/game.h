////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: game.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PIRATES_BASE_GAME_H_
#define PIRATES_BASE_GAME_H_

#include "pandaFramework.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"

namespace pirates {

namespace world {
class Ship;
class Planet;
}

namespace base {
class InputManager;

class Game {

  public:

    ~Game () {
        framework_.close_framework();
        reference_ = NULL;
        window_ = NULL;
    }

    static Game* reference () {
        if (reference_ == NULL)
            return (reference_ = new Game);
        return reference_;
    }

    void Init (int &argc, char **&argv);

    void Run () {
        framework_.main_loop();
    }
    
    PandaFramework& framework () {
        return framework_;
    }
    
    AsyncTaskManager& taskMgr () {
        return framework_.get_task_mgr();
    }

    WindowFramework*& window () {
        return window_;
    }

    NodePath& camera () {
        return camera_node_;
    }

    world::Ship* ship() { return ship_; }
    world::Planet* planet() { return planet_; }

  private:
    Game () : window_(NULL), inputmanager_(NULL) {}

    static Game *reference_;

    PandaFramework  framework_;
    WindowFramework *window_;
    NodePath camera_node_;

    InputManager *inputmanager_;

    world::Ship* ship_;
    world::Planet* planet_;

};

} // namespace base
} // namespace pirates

#endif

