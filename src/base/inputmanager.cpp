
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

InputManager::InputManager() : game_(Game::reference()), arrow_(new Arrow),
    current_handler_(NULL) {}

void InputManager::ClickDownEvent (const Event* e, void *data) {
    if (reference_->current_handler_)
        reference_->current_handler_->ClickDownEvent(e, 0);
}

void InputManager::ClickUpEvent (const Event* e, void *data) {
    if (reference_->current_handler_)
        reference_->current_handler_->ClickUpEvent(e, 0);
}

void InputManager::Init() {
    Game::reference()->window()->enable_keyboard();
    MouseWatcher *mouseWatcher = static_cast<MouseWatcher*>(Game::reference()->window()->get_mouse().node());
    //load_colliders();
    Game::reference()->framework().define_key("mouse1", "mouse_down", &ClickDownEvent, NULL);
    Game::reference()->framework().define_key("mouse1-up", "mouse_up", &ClickUpEvent, NULL);
    player_ship_ = Game::reference()->ship_;
    pirates::world::InputHandler *handler =
        new pirates::world::InputHandler(Game::reference(), mouseWatcher, player_ship_);
    handler->Setup();
    set_current_handler(handler);
}

} // namespace base
} // namespace pirates

