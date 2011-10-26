
#include "base/game.h"
#include "planet.h"
#include "camera.h"

#define CAMERA_VEL    500.0f
#define CAMERA_HEIGHT 150.0f

namespace pirates {

using base::Game;

namespace world {

    void Camera::Initialize() {
        LPoint3f planet_center = GAME()->planet()->center();
        up_ = LVector3f(0.0f,1.0f,0.0f);
        look_at_ = planet_center - GAME()->camera().get_pos();
        right_ = look_at_.cross(up_);
        right_.normalize();

        GAME()->camera().look_at(planet_center,up_);
        loadColliders();
    }

    void Camera::loadColliders () {
        // Makes a collision node to store the ray. Since it will come from the mouse, let's call it
        // "mouseRay
        PT(CollisionNode) picker_node = new CollisionNode("mouseRay");
        // As the camera is a Nodepath_ object, we may attatch other nodes to it. In this case, we
        // attatch the picker_node_ because it moves together with the camera.
        picker_node_path_ = GAME()->camera().attach_new_node(picker_node);
        picker_node->set_from_collide_mask(CollisionNode::get_default_collide_mask());
        picker_ray_ = new CollisionRay();
        picker_node->add_solid(picker_ray_);
        picker_handler_ = new CollisionHandlerQueue();
        traverser_.add_collider(picker_node_path_, picker_handler_);


        // Floor
        floor_node_ = new CollisionNode("floor");
        NodePath floor_node_path_ = GAME()->window()->get_render().attach_new_node(floor_node_);
        floor_node_path_.set_pos(0, 0, 0);
        
        // World
        world_node_ = new CollisionNode("sphere");
        NodePath world_node_path_ = GAME()->window()->get_render().attach_new_node(world_node_);
        world_node_path_.set_pos(0, 0, 0);

        // The planet
        planet_id_ = world_node_->add_solid(base::Game::reference()->planet()->get_collision());
    }

    CollisionEntry* Camera::pick(LPoint2f pos) {
        picker_ray_->set_from_lens(GAME()->window()->get_camera(0), pos.get_x(), pos.get_y());
        traverser_.traverse(GAME()->window()->get_render());
        if (picker_handler_->get_num_entries() > 0) {
            picker_handler_->sort_entries();
            return picker_handler_->get_entry(0);
        }
        else
            return NULL;
    }
    
    void Camera::MoveCamera(InputHandler::TWOD_MOV_DIR movement, float dt) {

        if(movement == InputHandler::NOTHING)
            return;

        LPoint3f planet_center = GAME()->planet()->center();
        InputHandler::TWOD_MOV_DIR x_movement = (InputHandler::TWOD_MOV_DIR) ((int) movement % 3);
        InputHandler::TWOD_MOV_DIR y_movement = (InputHandler::TWOD_MOV_DIR) ((int) movement - (int) x_movement);
        LPoint3f new_pos = GAME()->camera().get_pos();

        if(y_movement == InputHandler::Y_DOWN)
            new_pos -= up_*dt*CAMERA_VEL;
        else if (y_movement == InputHandler::Y_UP)
            new_pos += up_*dt*CAMERA_VEL;

        if(x_movement == InputHandler::X_DOWN)
            new_pos -= right_*dt*CAMERA_VEL;
        else if (x_movement == InputHandler::X_UP)
            new_pos += right_*dt*CAMERA_VEL;

        new_pos -= planet_center;
        new_pos.normalize();
        new_pos *= (GAME()->planet()->height_at(new_pos) + CAMERA_HEIGHT);

        GAME()->camera().set_pos(new_pos);
        look_at_ = new_pos - planet_center;

        if( (y_movement == InputHandler::Y_UP) || (y_movement == InputHandler::Y_DOWN) ) {
            up_ -= up_.project(look_at_);
            up_.normalize();
        }
        if( (x_movement == InputHandler::X_UP) || (x_movement == InputHandler::X_DOWN) ) {
            right_ -= right_.project(look_at_);
            right_.normalize();
        }

        GAME()->camera().look_at(planet_center,up_);
    }

    void Camera::AddPlaneAndRemoveWorld(LPoint3f target_pos) {
        PT(CollisionPlane) floor_plane = new CollisionPlane(Planef(LVector3f(target_pos), target_pos));
        plane_id_ = floor_node_->add_solid(floor_plane);
        world_node_->remove_solid(planet_id_);
    }

    void Camera::RemovePlaneAndAddWorld() {
        floor_node_->remove_solid(plane_id_);
        planet_id_ = world_node_->add_solid(GAME()->planet()->get_collision());
    }

}

}
