
#include "base/game.h"
#include "base/inputmanager.h"
#include "world/ship.h"
#include "world/inputhandler.h"
#include "world/arrow.h"
#include "world/planet.h"

#include "mouseWatcher.h"
#include "collisionPlane.h"

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus dragAux ( GenericAsyncTask* task, void* data ) {
    pirates::world::InputHandler::TaskData* taskdata = static_cast <pirates::world::InputHandler::TaskData*> (data);
    return taskdata->handler->Drag(task, taskdata->mouse_button);
}

namespace pirates {

namespace world {

using pirates::base::InputManager;

void InputHandler::Setup() {
    loadColliders();
    for(int i = 0; i < 3; i++) {
        task_data_[i].handler = this;
        task_data_[i].mouse_button = 1 << i;
    }
}

void InputHandler::ClickDownEvent(const Event *event, int mouse_button) {
    if (mouse_watcher_->has_mouse()) {
        LPoint2f pos = mouse_watcher_->get_mouse();
        PT(CollisionEntry) entry;
        bool drag = false;
        switch(mouse_button) {
        case InputManager::LEFT_BUTTON:
            mouse_pos_ = pos;
            drag = true;
            break;
        case InputManager::RIGHT_BUTTON:
            entry = pick(pos);
            if (entry) {
                target_pos_ = entry->get_surface_point(game_->window()->get_render());
                InputManager::reference()->get_arrow()->set_pos(target_pos_);
                PT(CollisionPlane) floor_plane =
                    new CollisionPlane(Planef(LVector3f(target_pos_), target_pos_));
                plane_id_ = floor_node_->add_solid(floor_plane);
                world_node_->remove_solid(planet_id_);
                drag = true;
            }
            break;
        default:
            return;
        }

        if(drag) {
            held_buttons_ |= mouse_button;
            TaskData* data;
            for(int i = 0; i < 3; i++)
                if(1 << i == mouse_button)
                    data = &task_data_[i];
            GenericAsyncTask *task = new GenericAsyncTask(string("MouseDrag"), (TaskFunc)&dragAux, (void*) data);
            printf("UP %d, Held: %d, TaskBtn %d\n", mouse_button, held_buttons_, data->mouse_button);
            GAME()->taskMgr().add(task);
        }
    }
}

void InputHandler::ClickUpEvent(const Event *event, int mouse_button) {
    if (mouse_watcher_->has_mouse()) {
        if(mouse_button == InputManager::RIGHT_BUTTON && (mouse_button & held_buttons_)){
            LPoint2f pos = mouse_watcher_->get_mouse();
            PT(CollisionEntry) entry = pick(pos);
            if (entry) {
                NodePath picked_obj = entry->get_into_node_path();
                LPoint3f vector_end = entry->get_surface_point(game_->window()->get_render());
			    LVector3f vector = vector_end - target_pos_;
                floor_node_->remove_solid(plane_id_);
                planet_id_ = world_node_->add_solid(base::Game::reference()->planet()->get_collision());
                if(vector.length()<=0.5f)
		  game_->ship()->set_new_route_dest(target_pos_);
                else
		  game_->ship()->set_new_route_dest(target_pos_,vector);
            }
        }
    }
    held_buttons_ &= (mouse_button ^ 0xFFFF);
    printf("DOWN %d, Held: %d\n", mouse_button, held_buttons_);
}



AsyncTask::DoneStatus InputHandler::Drag(GenericAsyncTask* task, int mouse_button) {
    if (!mouse_watcher_->has_mouse()) 
        return AsyncTask::DS_cont;

    if(!(held_buttons_ & mouse_button))
        return AsyncTask::DS_done;

    LPoint2f pos = mouse_watcher_->get_mouse();
    PT(CollisionEntry) entry;
    switch(mouse_button) {
    case InputManager::LEFT_BUTTON:
        mouse_pos_ = pos;
        break;
    case InputManager::RIGHT_BUTTON:
        entry = pick(pos);
        if (entry) {
            NodePath picked_obj = entry->get_into_node_path();
            LPoint3f vector_end = entry->get_surface_point(game_->window()->get_render());
		    LVector3f vector = vector_end - target_pos_;
            InputManager::reference()->get_arrow()->look_at(vector_end, vector);
        }
        break;
    default:
        break;
    }
    return AsyncTask::DS_cont;
}

// Let's check out the load_colliders function.
void InputHandler::loadColliders () {
    // Makes a collision node to store the ray. Since it will come from the mouse, let's call it
    // "mouseRay
    PT(CollisionNode) picker_node = new CollisionNode("mouseRay");
    // As the camera is a Nodepath_ object, we may attatch other nodes to it. In this case, we
    // attatch the picker_node_ because it moves together with the camera.
    picker_node_path_ = game_->camera().attach_new_node(picker_node);
    picker_node->set_from_collide_mask(CollisionNode::get_default_collide_mask());
    picker_ray_ = new CollisionRay();
    picker_node->add_solid(picker_ray_);
    picker_handler_ = new CollisionHandlerQueue();
    traverser_.add_collider(picker_node_path_, picker_handler_);


    // Floor
    floor_node_ = new CollisionNode("floor");
    NodePath floor_node_path_ = game_->window()->get_render().attach_new_node(floor_node_);
    floor_node_path_.set_pos(0, 0, 0);
    
    // World
    world_node_ = new CollisionNode("sphere");
    NodePath world_node_path_ = game_->window()->get_render().attach_new_node(world_node_);
    world_node_path_.set_pos(0, 0, 0);

    // The planet
    planet_id_ = world_node_->add_solid(base::Game::reference()->planet()->get_collision());
}

CollisionEntry* InputHandler::pick(LPoint2f pos) {
    picker_ray_->set_from_lens(game_->window()->get_camera(0), pos.get_x(), pos.get_y());
    traverser_.traverse(game_->window()->get_render());
    if (picker_handler_->get_num_entries() > 0) {
        picker_handler_->sort_entries();
        return picker_handler_->get_entry(0);
    }
    else
        return NULL;
}

}

}


