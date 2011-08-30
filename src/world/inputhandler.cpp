
#include "base/game.h"
#include "base/inputmanager.h"
#include "world/inputhandler.h"
#include "world/arrow.h"
#include "mouseWatcher.h"
#include "collisionRay.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "collisionNode.h"
#include "collisionPlane.h"
#include "plane.h"

namespace pirates {

namespace world {

using pirates::base::InputManager;

void InputHandler::Setup() {
    loadColliders();
}

void InputHandler::ClickDownEvent(const Event *event) {
    if (mouse_watcher_->has_mouse()) {
        LPoint2f pos = mouse_watcher_->get_mouse();
        PT(CollisionEntry) entry = pick(pos);
        if (entry) {
            target_pos_ = entry->get_surface_point(game_->window()->get_render());
            InputManager::reference()->get_arrow()->set_pos(target_pos_);
        }
    }
    else {
        puts("Mouse outsize window! How did you do that!?");
    }
}

void InputHandler::ClickUpEvent(const Event *event) {
    if (mouse_watcher_->has_mouse()){
        LPoint2f pos = mouse_watcher_->get_mouse();
        PT(CollisionEntry) entry = pick(pos);
        if (entry) {
            NodePath picked_obj = entry->get_into_node_path();
            LPoint3f vector_end = entry->get_surface_point(game_->window()->get_render());
			LVector3f vector = vector_end - target_pos_;
            printf("Down: %f, %f, %f \n", target_pos_.get_x(), target_pos_.get_y(), target_pos_.get_z() );
            printf("Up: %f, %f, %f \n", vector_end.get_x(), vector_end.get_y(), vector_end.get_z() );
            puts("Vector obtained:  " );
            printf("%f, %f, %f \n", vector.get_x(), vector.get_y(), vector.get_z() );
            InputManager::reference()->get_arrow()->look_at(vector_end);
            if(vector.length()<=0.5f)
                InputManager::reference()->player_ship_->set_new_route_dest(target_pos_);
            else
                InputManager::reference()->player_ship_->set_new_route_dest(target_pos_,vector);
        }
    }
    else {
        puts("Mouse outsize window! How did you do that!?");
    }
}

// Let's check out the load_colliders function.
void InputHandler::loadColliders () {
    // Loads the envinronment_ment model and stores it.
    // The load_model method from the window object needs the parent node that the model shall be
    // attached to and the path to the model file (.egg or .bam).
    environment_ = game_->window()->load_model(game_->framework().get_models(),
            "models/environment");
    // Reparents the model to render. This makes it visible.
    environment_.reparent_to(game_->window()->get_render());
    // Applies scale and position transforms to the model.
    environment_.set_color(1,1,1,1);
    environment_.set_scale(0.25, 0.25, 0.25);
    environment_.set_pos(-8, 42, 0);
    environment_.set_tag("pickable", "");
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
    picker_handler_ = new CollisionHandlerQueue();
    traverser_.add_collider(picker_node_path_, picker_handler_);
    // Floor
    floor_node_ = new CollisionNode("floor");
    floor_node_path_ = environment_.attach_new_node(floor_node_);
    floor_node_path_.set_pos(0, 0, 0);
    floor_plane_ = new CollisionPlane(Planef(LVector3f(0, 0, 1), LPoint3f(0, 0, 0)));
    floor_node_->add_solid(floor_plane_);
}

CollisionEntry* InputHandler::pick(LPoint2f pos) {
    picker_ray_->set_from_lens(game_->window()->get_camera(0), pos.get_x(), pos.get_y());
    traverser_.traverse(game_->window()->get_render());
    if (picker_handler_->get_num_entries() > 0) {
        picker_handler_->sort_entries();
        return picker_handler_->get_entry(0);
    }
    else {
        puts("No collision.");
        return NULL;
    }
}

}

}


