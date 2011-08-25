
#include "world/ship.h"
#include "base/game.h"
#include "genericAsyncTask.h"
#include "world/utils/routetracer.h"
#include "parametricCurve.h"
#include "nurbsCurve.h"
#include <string>

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus moveShipAux ( GenericAsyncTask* task, void* data ) {
    pirates::world::Ship* ship = static_cast <pirates::world::Ship*> (data);
    return ship->moveShip(task);
}

namespace pirates {

using base::Game;

namespace world {

//LVector3f Ship::dir(0.1, 0, 0);

Ship::Ship () : dir(0, 0, 0) {
    PandaFramework& framework = Game::reference()->framework();
    WindowFramework* window = Game::reference()->window();
    ship_node_ = window->load_model(framework.get_models(), "data/king");
    ship_node_.set_scale(5);
    ship_node_.reparent_to(window->get_render());
    ship_node_.set_color(0,0,0,1);
    ship_node_.set_pos(0, 0, 0);

    vel = 5.0f; // lol.
    LPoint3f ship_pos= ship_node_.get_pos();
    LVector3f ship_vel_norm= vel/vel.length();
    route_tracer_ = new utils::RouteTracer(ship_pos, vel.length(), ship_vel_norm);

    new_route_method_ = 0;
}

AsyncTask::DoneStatus Ship::moveShip ( GenericAsyncTask* task ) {
    //static LVector3f spd( 0, -0.1, 0);
    static double last = task->get_elapsed_time();
    float dt = task->get_elapsed_time() - last, time = task->get_elapsed_time();
    this->dir = LVector3f(cos(time), sin(time), 0);
    this->route_tracer_->get_next_pt(this->vel.length(), dt, this->new_point, this->new_tangent);
    this->ship_node_.set_pos(this->new_point);
    LVector3f new_tg_norm = this->new_tangent/this->new_tangent.length();
    //this->ship_node_.set_pos(this->ship_node_.get_pos()+this->vel*dt);
    switch(new_route_method_) {
        case 1:
            this->route_tracer_->trace_new_route(this->new_point, this->new_tangent.length(), new_tg_norm, this->new_route_dest_pos_);
            new_route_method_ = 0;
        break;
        case 2: 
            this->route_tracer_->trace_new_route(this->new_point, this->new_tangent.length(), new_tg_norm, this->new_route_dest_pos_, this->new_route_dest_vel_);
            new_route_method_ = 0;
        break;
        case 0:
        default:
        break;
    }
    last = time;
    return AsyncTask::DS_cont;
}

void Ship::taskInitialize ( AsyncTaskManager& taskMgr ) {
    GenericAsyncTask *task = new GenericAsyncTask(string("Ainda nao sei"),
        (TaskFunc)&moveShipAux, 
        (void*) this);
    taskMgr.add(task);
}

void Ship::set_new_route_dest(LPoint3f& dest) {
    new_route_dest_pos_ = dest;
    new_route_method_ = 1;
}

void Ship::set_new_route_dest(LPoint3f& dest_pos, LVector3f& dest_vel) {
    new_route_dest_pos_ = dest_pos;
    new_route_dest_vel_ = dest_vel;
    new_route_method_ = 2;
}


} // Namespace world
} // Namespace pirates
