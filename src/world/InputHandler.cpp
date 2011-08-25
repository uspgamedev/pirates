
#include "world/inputhandler.h"
#include "base/game.h"
#include "mouseWatcher.h"
#include "collisionRay.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "collisionNode.h"
#include "collisionPlane.h"
#include "plane.h"

namespace pirates {

namespace world {

void InputHandler::Setup() {
    loadColliders();
}

void InputHandler::ClickDownEvent(const Event *event) {

}

// Let's check out the load_colliders function.
void InputHandler::loadColliders () {
    // Loads the envinronment_ment model and stores it.
    // The load_model method from the window object needs the parent node that the model shall be
    // attached to and the path to the model file (.egg or .bam).
    envinronment_ = game_->window()->load_model(game_->framework().get_models(),
            "models/envinronment_");
    // Reparents the model to render. This makes it visible.
    envinronment_.reparent_to(game_->window()->get_render());
    // Applies scale and position transforms to the model.
    envinronment_.set_color(1,1,1,1);
    envinronment_.set_scale(0.25, 0.25, 0.25);
    envinronment_.set_pos(-8, 42, 0);
    envinronment_.set_tag("pickable", "");
    // Makes a collision node to store the ray. Since it will come from the mouse, let's call it
    // "mouseRay
    picker_node_ = new CollisionNode("mouseRay");
    // As the camera is a Nodepath_ object, we may attatch other nodes to it. In this case, we
    // attatch the picker_node_ because it moves together with the camera.
    picker_node_path_ = game_->camera().attach_new_node(picker_node_);
    // TODO...
    picker_node_->set_from_collide_mask(CollisionNode::get_default_collide_mask());
    picker_ray_ = new CollisionRay();
    picker_node_->add_solid(picker_ray_);
    pickerHandler = new CollisionHandlerQueue();
    traverser_.add_collider(picker_node_path_, pickerHandler);
    // Floor
    floor_node_ = new CollisionNode("floor");
    floor_node_path_ = envinronment_.attach_new_node(floor_node_);
    floor_node_path_.set_pos(0, 0, 0);
    floor_plane_ = new CollisionPlane(Planef(LVector3f(0, 0, 1), LPoint3f(0, 0, 0)));
    floor_node_->add_solid(floor_plane_);
}


}

}


