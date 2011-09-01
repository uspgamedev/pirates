
#ifndef PIRATES_WORLD_INPUTHANDLER_H_
#define PIRATES_WORLD_INPUTHANDLER_H_

#include "base/inputhandler.h"

#include "nodePath.h"
#include "collisionTraverser.h"

class CollisionRay;
class CollisionPlane;
class CollisionHandlerQueue;
class CollisioNode;
class CollisionEntry;
class MouseWatcher;

namespace pirates {
namespace world {
class Ship;

class InputHandler : public pirates::base::InputHandler {
  public:
    InputHandler(pirates::base::Game *game, MouseWatcher *mouse_watcher, Ship *ship)
        : pirates::base::InputHandler(game, mouse_watcher), ship_(ship),
          traverser_("picker_traverser"), held_buttons_(0) {}

    typedef struct {
        InputHandler* handler;
        int mouse_button;
    } TaskData;

    void Setup();
    void ClickDownEvent(const Event *event, int mouse_button);
    void ClickUpEvent(const Event *event, int mouse_button);
    AsyncTask::DoneStatus Drag(GenericAsyncTask* task, int mouse_button);

  private:
    int                         plane_id_;
    int                         planet_id_;
    Ship                        *ship_;
    PT(CollisionRay)            picker_ray_;
    CollisionTraverser          traverser_;
    PT(CollisionHandlerQueue)   picker_handler_;
    PT(CollisionNode)           floor_node_;
    PT(CollisionNode)           world_node_;
    NodePath                    picker_node_path_;
    LPoint3f                    target_pos_, speed_dir_;
    LPoint2f                    mouse_pos_;
    int                         held_buttons_;
    TaskData                    task_data_[3];

    void loadColliders();
    CollisionEntry* pick(LPoint2f pos);

};
} // namespace world
} // namespace pirates

#endif

