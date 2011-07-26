
#include "boat.h"

namespace pirates {

Boat::Boat (std::string &modelpath, WindowFramework*& window, PandaFramework& framework ) {
    NodePath m;
    m = window->load_model(framework.get_models(), "panda-model");
    m.set_scale(0.005);
    m.reparent_to(window->get_render());
    m.set_pos(5, 5, 5);
    boat_node_ = m;
}

AsyncTask::DoneStatus Boat::moveBoat ( GenericAsyncTask* task, void* data ) {
  double time = globalClock_->get_real_time();
  double angledegrees = time * 6.0;
  double angleradians = angledegrees * (3.14 / 180.0);
  boat_node_.set_pos(20*sin(angleradians),-20.0*cos(angleradians),3);
  boat_node_.set_hpr(angledegrees, 0, 0);
 
  return AsyncTask::DS_cont;
}

void Boat::taskInicialize() {
    PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
    PT(ClockObject) globalClock = ClockObject::get_global_clock();
    taskMgr_ = taskMgr;
    globalClock_ = globalClock;
    taskMgr_->add(new GenericAsyncTask("Ainda nao sei",
        &moveBoat, (void*) NULL));
}


}
