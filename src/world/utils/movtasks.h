
#ifndef PIRATES_WORLD_UTILS_MOVTASKS_H_
#define PIRATES_WORLD_UTILS_MOVTASKS_H_

#include "asyncTask.h"
#include "pandaFramework.h"
#include "texture.h"

namespace pirates {
namespace world {

class WorldActor;
class Planet;

namespace utils {

class Navigator;

class HueCyclingMovTask : public AsyncTask {

  public:
    HueCyclingMovTask(const string& movtask_name, WorldActor* actor);

  protected:
    AsyncTask::DoneStatus do_task();
    void upon_death(AsyncTaskManager *manager, bool clean_exit);

  private:
    const Colorf UpdateSpeedBasedHueAndReturnColor(const Navigator* navi, const float dt);

    WorldActor* actor_;
    const Planet* planet_;
    double last_time_;

    float speed_based_hue_;
    bool warned_once_about_texture_blend_stage_missing_;
};

}
}
}

#endif