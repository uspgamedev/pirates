
#ifndef PIRATES_WORLD_INPUTHANDLER_H_
#define PIRATES_WORLD_INPUTHANDLER_H_

#include "base/inputhandler.h"
#include "base/runnable.h"

#include "nodePath.h"
#include "collisionTraverser.h"
#include "collisionNode.h"
#include "collisionHandlerQueue.h"
#include "collisionRay.h"

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

    enum TWOD_MOV_DIR {
        NOTHING         = 0,
        X_UP            = 1,
        X_DOWN          = 2,
        Y_UP            = 3,
        X_UP_Y_UP       = 4,
        X_DOWN_Y_UP     = 5,
        Y_DOWN          = 6,
        X_UP_Y_DOWN     = 7,
        X_DOWN_Y_DOWN   = 8
    };

    InputHandler(pirates::base::Game *game, MouseWatcher *mouse_watcher, Ship *ship)
        : pirates::base::InputHandler(game, mouse_watcher), ship_(ship),
          traverser_("picker_traverser"), held_buttons_(0) {}

    typedef struct {
        InputHandler* handler;
        int mouse_button;
    } TaskData;

    void Update(float dt);
    void Setup();
    void ClickDownEvent(const Event *event, int mouse_button);
    void ClickUpEvent(const Event *event, int mouse_button);
    AsyncTask::DoneStatus Drag(GenericAsyncTask* task, int mouse_button);

  private:
    Ship                        *ship_;
    CollisionTraverser          traverser_;
    LPoint3f                    target_pos_, speed_dir_;
    LPoint2f                    mouse_pos_;
    int                         held_buttons_;
    TaskData                    task_data_[3];

    //TODO: futuro camera.h/camera.cpp
    LVector3f                   look_at_;
    LVector3f                   up_;
    LVector3f                   right_;

    void loadColliders();
    CollisionEntry* pick(LPoint2f pos);

};
} // namespace world
} // namespace pirates

#endif

