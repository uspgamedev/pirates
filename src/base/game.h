////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: game.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GAME_H_
#define GAME_H_

#include "pandaFramework.h"

namespace pirates {
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

    WindowFramework*& window () {
        return window_;
    }

  private:
    
    Game () : window_(NULL), inputmanager_(NULL) {}

    static Game *reference_;

    PandaFramework  framework_;
    WindowFramework *window_;

    InputManager *inputmanager_;

};

} // namespace base
} // namespace pirates

#endif

