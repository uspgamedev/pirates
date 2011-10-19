
#ifndef PIRATES_WORLD_CAMERA_H_
#define PIRATES_WORLD_CAMERA_H_

#include "lpoint3.h"
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

namespace pirates {
namespace world {

class Camera {

    public:
        Camera() {}
        void Initialize ();
        void MoveCamera(int cam, float dt);

    private:
        LPoint3f        position_;
        LVector3f       up_;
        LVector3f       right_;
        LVector3f       look_at_;
};


} // namespace world
} // namespace pirates

#endif