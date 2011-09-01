
#include "base/inputmanager.h"
#include "base/game.h"
#include "world/arrow.h"


namespace pirates {

using world::Arrow;

namespace base {

InputManager* InputManager::reference_ = NULL;


struct MouseClickData {
    InputManager::MouseButton mouse_button;
};

static struct MouseClickData mouse1, mouse2, mouse3;

InputManager::InputManager() : arrow_(new Arrow), current_handler_(NULL) {}

void InputManager::ClickDownEvent (const Event* e, void *data) {
    struct MouseClickData mousedata = *((struct MouseClickData*)data);
    if (reference_->current_handler_)
        reference_->current_handler_->ClickDownEvent(e, mousedata.mouse_button);
}

void InputManager::ClickUpEvent (const Event* e, void *data) {
    struct MouseClickData mousedata = *((struct MouseClickData*)data);
    if (reference_->current_handler_)
        reference_->current_handler_->ClickUpEvent(e, mousedata.mouse_button);
}

void InputManager::Init() {
    mouse1.mouse_button = InputManager::LEFT_BUTTON;
    mouse2.mouse_button = InputManager::RIGHT_BUTTON;
    mouse3.mouse_button = InputManager::MIDDLE_BUTTON;

    Game* game = Game::reference();

    game->window()->enable_keyboard();

    game->framework().define_key("mouse1", "mouse_down", &ClickDownEvent, &mouse1);
    game->framework().define_key("mouse1-up", "mouse_up", &ClickUpEvent, &mouse1);

    // In panda, the middle button is mouse2
    game->framework().define_key("mouse2", "mouse_down", &ClickDownEvent, &mouse3);
    game->framework().define_key("mouse2-up", "mouse_up", &ClickUpEvent, &mouse3);

    game->framework().define_key("mouse3", "mouse_down", &ClickDownEvent, &mouse2);
    game->framework().define_key("mouse3-up", "mouse_up", &ClickUpEvent, &mouse2);
}

} // namespace base
} // namespace pirates

