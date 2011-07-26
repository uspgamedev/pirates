
#include "base/inputmanager.h"
#include "base/game.h"

namespace pirates {
namespace base {

InputManager* InputManager::reference_ = NULL;

InputManager::InputManager () : game_(Game::reference()) {}

void InputManager::Init () {
    Game::reference()->window()->enable_keyboard();
}

//void InputManager::ClickDownEvent (const Event* e, void *data) {
    /*puts("Mouse click detected!");
    if (mouseWatcher->has_mouse()){
        LPoint2f pos = mouseWatcher->get_mouse();
        //printf("Click at (%f, %f)\n", pos.get_x(), pos.get_y());
        PT(CollisionEntry) entry = pick(pos);

        if (entry) {
            NodePath pickedObj = entry->get_into_node_path();
            LPoint3f hit_pos = entry->get_surface_point(window->get_render());
            smiley.set_x(hit_pos.get_x());
            smiley.set_y(hit_pos.get_y());
        }
    }
    else {
        puts("Mouse outsize window! How did you do that!?");
    }*/
//}

} // namespace base
} // namespace pirates

