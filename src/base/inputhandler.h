

#ifndef PIRATES_BASE_INPUTHANDLER_H_
#define PIRATES_BASE_INPUTHANDLER_H_

#include "base/game.h"

class Event;
class MouseWatcher;

namespace pirates {

namespace base {

class Game;

class InputHandler {

  public:

    virtual void Setup() = 0;

    virtual void ClickDownEvent(const Event *event) = 0;

    virtual void ClickUpEvent(const Event *event) = 0;

  protected:

    InputHandler(Game *game, MouseWatcher *mouse_watcher) : game_(game), mouse_watcher_(mouse_watcher) {}


    Game            *game_;
    MouseWatcher    *mouse_watcher_;


};

}

}

#endif


