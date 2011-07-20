#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include <cstdlib>
#include "eventhandler.h"
#include "game.h"

class InputManager {

  public:
    
    ~InputManager() {
        reference_ = NULL;
    }

    static InputManager* reference() {
        if (reference_ == NULL)
            return (reference_ = new InputManager());
        else
            return reference_;
    }

    void Init () {
        Game::reference()->window()->enable_keyboard();
    }

  private:

    InputManager() : game_(Game::reference()) {}

    Game*& game_;

    static InputManager* reference_;

    static void ClickDownEvent(const Event* e, void *data) {}

    static void ClickUpEvent(const Event* e, void *data) {}

};

#endif

