
#include "world/ship.h"
#include "base/game.h"
#include "genericAsyncTask.h"
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
}

AsyncTask::DoneStatus Ship::moveShip ( GenericAsyncTask* task ) {
    //static LVector3f spd( 0, -0.1, 0);
    static double last = task->get_elapsed_time();
    double dt = task->get_elapsed_time() - last, time = task->get_elapsed_time();
    dir = LVector3f(cos(time), sin(time), 0);
    this->ship_node_.set_pos(this->ship_node_.get_pos()+5*dt*dir);
    last = time;
    return AsyncTask::DS_cont;
}

void Ship::taskInitialize ( AsyncTaskManager& taskMgr ) {
    GenericAsyncTask *task = new GenericAsyncTask(string("Ainda nao sei"),
        (TaskFunc)&moveShipAux, 
        (void*) this);
    taskMgr.add(task);
}


} // Namespace world
} // Namespace pirates
