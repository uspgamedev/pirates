
#ifndef PIRATES_BASE_INPUTMANAGER_H_
#define PIRATES_BASE_INPUTMANAGER_H_

#include <cstdlib>
#include "base/inputhandler.h"

class Event;

namespace pirates {

namespace world {
class Arrow;
}

namespace base {

class Game;
class InputHandler;

class InputManager {
  public:
    enum MouseButton {
        LEFT_BUTTON = 1,
        RIGHT_BUTTON = 2,
        MIDDLE_BUTTON = 4
    };

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

    void set_current_handler(InputHandler *handler) {
        if (current_handler_)
            delete current_handler_;
        current_handler_ = handler;
    }

    void Init ();

  private:
    InputManager();

    pirates::world::Arrow*  arrow_;
    InputHandler*           current_handler_;

    static InputManager*    reference_;

    static void ClickDownEvent(const Event* e, void *data);
    static void ClickUpEvent(const Event* e, void *data);
};

} // namespace base
} // namespace pirates

#endif

