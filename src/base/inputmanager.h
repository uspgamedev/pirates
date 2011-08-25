
#ifndef PIRATES_BASE_INPUTMANAGER_H_
#define PIRATES_BASE_INPUTMANAGER_H_

#include <cstdlib>
#include "world/ship.h"

class Event;

namespace pirates {

namespace world {
class Arrow;
}

namespace base {

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

    ::pirates::world::Arrow* get_arrow() {
        return arrow_;
    }

    void Init ();

    world::Ship* player_ship_;

  private:

    InputManager();
    Game* game_;
    ::pirates::world::Arrow* arrow_;

    static InputManager* reference_;

    static void ClickDownEvent(const Event* e, void *data) {}

    static void ClickUpEvent(const Event* e, void *data) {}

};

} // namespace base
} // namespace pirates

#endif

