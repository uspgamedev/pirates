
#include "boat.h"
#include "genericAsyncTask.h"
#include <string>

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus moveBoatAux ( GenericAsyncTask* task, void* data ) {
    pirates::Boat* boat = static_cast <pirates::Boat*> (data);
    return boat->moveBoat(task);
}


namespace pirates {

Boat::Boat (std::string &modelpath, WindowFramework*& window, PandaFramework& framework ) {
    NodePath m;
    m = window->load_model(framework.get_models(), "panda-model");
    m.set_scale(0.005);
    m.reparent_to(window->get_render());
    m.set_pos(5, 5, 5);
    boat_node_ = m;
}

AsyncTask::DoneStatus Boat::moveBoat ( GenericAsyncTask* task ) {
    static LVector3f spd( 0, -0.1, 0);
    static double last = task->get_elapsed_time() + last;
    double dt = task->get_elapsed_time() - last;
    LVector3f dir = ( 0, 1, 0 );
    this->boat_node_.set_pos(this->boat_node_.get_pos()+dt*spd);

    return AsyncTask::DS_cont;
}

void Boat::taskInicialize ( AsyncTaskManager& taskMgr ) {
    GenericAsyncTask *task = new GenericAsyncTask(string("Ainda nao sei"),
        (TaskFunc)&moveBoatAux, 
        (void*) this);
    taskMgr.add(task);
}


}
