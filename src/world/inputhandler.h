
#ifndef PIRATES_WORLD_INPUTHANDLER_H_
#define PIRATES_WORLD_INPUTHANDLER_H_

#include "base/inputhandler.h"

#include "nodePath.h"
#include "collisionTraverser.h"
#include "collisionSphere.h"

class CollisionRay;
class CollisionPlane;
//class CollisionSphere;
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
          traverser_("picker_traverser") {}

    void Setup();

    void ClickDownEvent(const Event *event);

    void ClickUpEvent(const Event *event);

  private:

    int                         plane_id_;
    int                         sphere_id_;
    Ship                        *ship_;
    PT(CollisionRay)            picker_ray_;
    PT(CollisionPlane)          floor_plane_;
    PT(CollisionSphere)         world_;
    CollisionTraverser          traverser_;
    PT(CollisionHandlerQueue)   picker_handler_;
    PT(CollisionNode)           picker_node_;
    PT(CollisionNode)           floor_node_;
    PT(CollisionNode)           world_node_;
    NodePath                    picker_node_path_;
    NodePath                    floor_node_path_;
    NodePath                    world_node_path_;
    NodePath                    environment_;
    LPoint3f                    target_pos_, speed_dir_;

    void loadColliders();

    CollisionEntry* pick(LPoint2f pos);

};

}

}

#endif

