////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: game.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GAME_H_
#define GAME_H_

#include "pandaFramework.h"

namespace pirates {

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

    void init (int &argc, char **&argv) {
        framework_.open_framework(argc, argv);
        framework_.set_window_title("Pirates by USPGameDev");
        window_ = framework_.open_window();
    }

    void run () {
        framework_.main_loop();
    }
    
    PandaFramework& framework () {
        return framework_;
    }

    WindowFramework*& window () {
        return window_;
    }

  private:
    
    Game () : window_(NULL) {}

    static Game *reference_;

    PandaFramework  framework_;
    WindowFramework *window_;

};

}

#endif

