#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include <cstdlib>

class Event;

namespace pirates {

class Game;

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

    void Init ();

  private:

    InputManager();
    Game* game_;

    static InputManager* reference_;

    static void ClickDownEvent(const Event* e, void *data) {}

    static void ClickUpEvent(const Event* e, void *data) {}

};

}

#endif

