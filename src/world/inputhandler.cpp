
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

#include "geomVertexData.h"
#include "geomVertexFormat.h"
#include "geom.h"
#include "geomNode.h"
#include "geomVertexWriter.h"
#include "geomTristrips.h"
#include "geomTriangles.h"


namespace pirates {

namespace world {

using pirates::base::InputManager;

void InputHandler::Setup() {
    loadColliders();
}

void InputHandler::ClickDownEvent(const Event *event, int mouse_button) {
    if (mouse_watcher_->has_mouse() && mouse_button == InputManager::RIGHT_BUTTON) {
        LPoint2f pos = mouse_watcher_->get_mouse();
        PT(CollisionEntry) entry = pick(pos);
        if (entry) {
            target_pos_ = entry->get_surface_point(game_->window()->get_render());
            InputManager::reference()->get_arrow()->set_pos(target_pos_);
            floor_plane_ = new CollisionPlane(Planef(LVector3f(target_pos_), target_pos_));
            //floor_plane_ = new CollisionPlane(Planef(LVector3f(0,0,1), target_pos_));
            plane_id_ = floor_node_->add_solid(floor_plane_);
            world_node_->remove_solid(sphere_id_);
        }
    }
}

void InputHandler::ClickUpEvent(const Event *event, int mouse_button) {
    if (mouse_watcher_->has_mouse() && mouse_button == InputManager::RIGHT_BUTTON){
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
            InputManager::reference()->get_arrow()->look_at(vector_end, vector);
            floor_node_->remove_solid(plane_id_);
            sphere_id_ = world_node_->add_solid(world_);
            if(vector.length()<=0.5f)
                InputManager::reference()->player_ship_->set_new_route_dest(target_pos_);
            else
                InputManager::reference()->player_ship_->set_new_route_dest(target_pos_,vector);
        }
    }
}

// Let's check out the load_colliders function.
void InputHandler::loadColliders () {


    PT(GeomVertexData) vdata;
    vdata = new GeomVertexData("triangle", GeomVertexFormat::get_v3n3c4(), Geom::UH_static);
    GeomVertexWriter vertex, normal, color, texcoord;
    vertex = GeomVertexWriter(vdata, "vertex");
    normal = GeomVertexWriter(vdata, "normal");
    color = GeomVertexWriter(vdata, "color");

    // Making the primitive
    //PT(GeomTristrips) prim;
    PT(GeomTriangles) prim;
    //prim = new GeomTristrips(Geom::UH_static);
    prim = new GeomTriangles(Geom::UH_static);

    // THE GEOM!
    PT(Geom) geom;
    geom = new Geom(vdata);
    geom->add_primitive(prim);
     
    // DA NODE!
    PT(GeomNode) node;
    node = new GeomNode("gnode");
    node->add_geom(geom);

    environment_ = game_->window()->get_render().attach_new_node(node);


    // Makes a collision node to store the ray. Since it will come from the mouse, let's call it
    // "mouseRay
    picker_node_ = new CollisionNode("mouseRay");
    // As the camera is a Nodepath_ object, we may attatch other nodes to it. In this case, we
    // attatch the picker_node_ because it moves together with the camera.
    picker_node_path_ = game_->camera().attach_new_node(picker_node_);
    picker_node_->set_from_collide_mask(CollisionNode::get_default_collide_mask());
    picker_ray_ = new CollisionRay();
    picker_node_->add_solid(picker_ray_);
    picker_handler_ = new CollisionHandlerQueue();
    traverser_.add_collider(picker_node_path_, picker_handler_);
    // Floor
    floor_node_ = new CollisionNode("floor");
    world_node_ = new CollisionNode("sphere");
    world_node_path_ = game_->window()->get_render().attach_new_node(world_node_);
    world_node_path_.set_pos(0, 0, 0);
    floor_node_path_ = game_->window()->get_render().attach_new_node(floor_node_);
    floor_node_path_.set_pos(0, 0, 0);
    floor_plane_ = new CollisionPlane(Planef(LVector3f(0, 0, 1), LPoint3f(0, 0, 0)));
    // World
    world_ = new CollisionSphere(0.0f, 0.0f, 0.0f, 40.0f);
    sphere_id_ = world_node_->add_solid(world_);
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


