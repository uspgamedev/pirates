
#include "base/game.h"
#include "camera.h"
#include "base/inputmanager.h"
#include "world/ship.h"
#include "world/utils/navigator.h"
#include "world/inputhandler.h"
#include "world/arrow.h"
#include "world/planet.h"

#include "mouseWatcher.h"
#include "collisionPlane.h"

//TODO: futuro camera.h/camera.cpp
#define CAMERA_VEL    500.0f
#define CAMERA_HEIGHT 150.0f

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus dragAux ( GenericAsyncTask* task, void* data ) {
    pirates::world::InputHandler::TaskData* taskdata = static_cast <pirates::world::InputHandler::TaskData*> (data);
    return taskdata->handler->Drag(task, taskdata->mouse_button);
}

namespace pirates {

namespace world {

using pirates::base::InputManager;

void InputHandler::Setup() {
    for(int i = 0; i < 3; i++) {
        task_data_[i].handler = this;
        task_data_[i].mouse_button = 1 << i;
    }
    base::RunnableManager::reference()->Add(this);
}

void InputHandler::Update(float dt) {

    if(mouse_watcher_->has_mouse()) {
        TWOD_MOV_DIR movement = NOTHING;

        if(mouse_watcher_->get_mouse_y() < -0.9f)
            movement = TWOD_MOV_DIR::Y_DOWN;
        else if(mouse_watcher_->get_mouse_y() > 0.9f)
            movement = TWOD_MOV_DIR::Y_UP;

        if(mouse_watcher_->get_mouse_x() < -0.9f)
            movement = (TWOD_MOV_DIR) ((int) movement + (int) TWOD_MOV_DIR::X_UP);
        else if(mouse_watcher_->get_mouse_x() > 0.9f)
            movement = (TWOD_MOV_DIR) ((int) movement + (int) TWOD_MOV_DIR::X_DOWN);

        GAME()->camera_man()->MoveCamera(movement, dt);
    }

    return;
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
            entry = GAME()->camera_man()->pick(pos);
            if (entry) {
                target_pos_ = entry->get_surface_point(GAME()->window()->get_render());
                InputManager::reference()->get_arrow()->set_pos(target_pos_);
                GAME()->camera_man()->AddPlaneAndRemoveWorld(target_pos_);
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
            printf("DOWN %d, Held: %d, TaskBtn %d\n", mouse_button, held_buttons_, data->mouse_button);
            GAME()->taskMgr().add(task);
        }
    }
}

void InputHandler::ClickUpEvent(const Event *event, int mouse_button) {
    if (mouse_watcher_->has_mouse()) {
        if(mouse_button == InputManager::RIGHT_BUTTON && (mouse_button & held_buttons_)){
            LPoint2f pos = mouse_watcher_->get_mouse();
            PT(CollisionEntry) entry = GAME()->camera_man()->pick(pos);
            if (entry) {
                NodePath picked_obj = entry->get_into_node_path();
                LPoint3f vector_end = entry->get_surface_point(GAME()->window()->get_render());
			    LVector3f vector = vector_end - target_pos_;
                GAME()->camera_man()->RemovePlaneAndAddWorld();
		        game_->ship()->navigator()->TraceNewRouteTo(target_pos_,vector);
            }
        }
    }
    held_buttons_ &= (mouse_button ^ 0xFFFF);
    printf("UP %d, Held: %d\n", mouse_button, held_buttons_);
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
        entry = GAME()->camera_man()->pick(pos);
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

}

}


