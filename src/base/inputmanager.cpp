
#include "base/inputmanager.h"
#include "base/game.h"
#include "world/arrow.h"
#include "world/inputhandler.h"
#include "mouseWatcher.h"
#include "collisionRay.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "collisionNode.h"
#include "collisionPlane.h"
#include "plane.h"
#include "world/ship.h"

namespace pirates {

using world::Arrow;

namespace base {

InputManager* InputManager::reference_ = NULL;


struct MouseClickData {
    int mouse_button;
};

static struct MouseClickData mouse1, mouse2, mouse3;

InputManager::InputManager() : game_(Game::reference()), arrow_(new Arrow),
    current_handler_(NULL) {}

void InputManager::ClickDownEvent (const Event* e, void *data) {
    struct MouseClickData mousedata = *((struct MouseClickData*)data);
    if (reference_->current_handler_)
        reference_->current_handler_->ClickDownEvent(e, mousedata.mouse_button);
    printf("Click down %d\n", mousedata.mouse_button);
}

void InputManager::ClickUpEvent (const Event* e, void *data) {
    struct MouseClickData mousedata = *((struct MouseClickData*)data);
    if (reference_->current_handler_)
        reference_->current_handler_->ClickUpEvent(e, mousedata.mouse_button);
}

void InputManager::Init() {
    Game::reference()->window()->enable_keyboard();
    MouseWatcher *mouseWatcher = static_cast<MouseWatcher*>(Game::reference()->window()->get_mouse().node());
    //load_colliders();
    mouse1.mouse_button = InputManager::LEFT_BUTTON;
    mouse2.mouse_button = InputManager::RIGHT_BUTTON;
    mouse3.mouse_button = InputManager::MIDDLE_BUTTON;

    Game::reference()->framework().define_key("mouse1", "mouse_down", &ClickDownEvent, &mouse1);
    Game::reference()->framework().define_key("mouse1-up", "mouse_up", &ClickUpEvent, &mouse1);

    // In panda, the middle button is mouse2
    Game::reference()->framework().define_key("mouse2", "mouse_down", &ClickDownEvent, &mouse3);
    Game::reference()->framework().define_key("mouse2-up", "mouse_up", &ClickUpEvent, &mouse3);

    Game::reference()->framework().define_key("mouse3", "mouse_down", &ClickDownEvent, &mouse2);
    Game::reference()->framework().define_key("mouse3-up", "mouse_up", &ClickUpEvent, &mouse2);

    player_ship_ = Game::reference()->ship();
    pirates::world::InputHandler *handler =
        new pirates::world::InputHandler(Game::reference(), mouseWatcher, player_ship_);
    handler->Setup();
    set_current_handler(handler);
}

} // namespace base
} // namespace pirates

