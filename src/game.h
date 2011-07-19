
#ifndef GAME_H_
#define GAME_H_

#include "pandaFramework.h"

class Game {
public:

    ~Game () {
        framework_.close_framework();
        reference_ = NULL;
    }

    static Game* init (int& argc, char& **argv) {
        return new Game(argc, argv);
    }

    static Game* reference () {
        return reference_;
    }
    
    PandaFramework& framework () {
        return framework_;
    }

private:
    
    Game (int& argc, char& **argv) {
        framework_.open_framework(argc, argv);
    }

    static Game *reference_;

    PandaFramework framework_;

};

#endif

