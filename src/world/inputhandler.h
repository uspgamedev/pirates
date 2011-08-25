
#ifndef PIRATES_WORLD_INPUTHANDLER_H_
#define PIRATES_WORLD_INPUTHANDLER_H_

#include "base/inputhandler.h"
#include "nodePath.h"
#include "collisionTraverser.h"

class CollisionRay;
class CollisionPlane;
class CollisionHandlerQueue;
class CollisioNode;

namespace pirates {

namespace world {

class Ship;

class InputHandler : public pirates::base::InputHandler {

  public:

    InputHandler(Game *game, MouseWatcher *mouse_watcher, Ship *ship)
        : pirates::base::InputHandler(game, mouse_watcher), ship_(ship),
          traverser_("picker_traverser") {}

    void Setup();

    void ClickDownEvent(const Event *event);

    void ClickUpEvent(const Event *event);

  private:

    Ship                        *ship_;
    PT(CollisionRay)            picker_ray_;
    PT(CollisionPlane)          floor_plane_;
    CollisionTraverser          traverser_;
    PT(CollisionHandlerQueue)   picker_handler_;
    PT(CollisionNode)           picker_node_;
    PT(CollisionNode)           floor_node_;
    NodePath                    picker_node_path_;
    NodePath                    floor_node_path_;
    NodePath                    environment_;

    void loadColliders();

};

}

}

#endif

