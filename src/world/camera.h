
#ifndef PIRATES_WORLD_CAMERA_H_
#define PIRATES_WORLD_CAMERA_H_

#include "inputhandler.h"
#include "lpoint3.h"
#include "nodePath.h"
#include "collisionTraverser.h"
#include "collisionNode.h"
#include "collisionHandlerQueue.h"
#include "collisionRay.h"
#include "collisionPlane.h"

class CollisionRay;
class CollisionPlane;
class CollisionHandlerQueue;
class CollisioNode;
class CollisionEntry;

namespace pirates {
namespace world {

class Camera {

    public:
        Camera() {}
        void Initialize ();
        void MoveCamera(InputHandler::TWOD_MOV_DIR movement, float dt);
        void loadColliders ();
        CollisionEntry* pick(LPoint2f pos);
        void AddPlaneAndRemoveWorld(LPoint3f target_pos);
        void RemovePlaneAndAddWorld();
        int get_planet_id() {return planet_id_;}

    private:
        int                         plane_id_;
        int                         planet_id_;
        PT(CollisionRay)            picker_ray_;
        CollisionTraverser          traverser_;
        PT(CollisionHandlerQueue)   picker_handler_;
        PT(CollisionNode)           floor_node_;
        PT(CollisionNode)           world_node_;
        NodePath                    picker_node_path_;

        LPoint3f        position_;
        LVector3f       up_;
        LVector3f       right_;
        LVector3f       look_at_;
};


} // namespace world
} // namespace pirates

#endif