
#include "world/ship.h"
#include "genericAsyncTask.h"
#include <string>

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus moveShipAux ( GenericAsyncTask* task, void* data ) {
    pirates::world::Ship* ship = static_cast <pirates::world::Ship*> (data);
    return ship->moveShip(task);
}


namespace pirates {

namespace world {


Ship::Ship (std::string &modelpath, WindowFramework*& window, PandaFramework& framework ) {
    NodePath m;
    m = window->load_model(framework.get_models(), "data/king");
    m.set_scale(0.5);
    m.reparent_to(window->get_render());
    m.set_color(0,0,0,1);
    m.set_pos(5, 5, 5);
    ship_node_ = m;
}

AsyncTask::DoneStatus Ship::moveShip ( GenericAsyncTask* task ) {
    static LVector3f spd( 0, -0.1, 0);
    static double last = task->get_elapsed_time() + last;
    double dt = task->get_elapsed_time() - last;
    LVector3f dir = ( 0, 1, 0 );
    this->ship_node_.set_pos(this->ship_node_.get_pos()+dt*spd);

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
