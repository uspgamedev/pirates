
#ifndef PIRATES_WORLD_UTILS_ROUTETRACER_H_
#define PIRATES_WORLD_UTILS_ROUTETRACER_H_

#include "pandaFramework.h"
#include "nurbsCurve.h"
#include "asyncTask.h"
#include "world/planet.h"

namespace pirates {
namespace world {

class WorldActor;

namespace utils {

class Navigator;
class HueCyclingMovTask : public AsyncTask {

  public:
    HueCyclingMovTask(WorldActor* actor);

  protected:
    AsyncTask::DoneStatus do_task();
    void upon_death(AsyncTaskManager *manager, bool clean_exit);

  private:
    WorldActor* actor_;
    Planet* planet_;
    double last_time_;

    bool warned_once_about_texture_blend_stage_missing_;
};


class Navigator {

  public:

    /*** PUBLIC METHODS ***/

    Navigator(const WorldActor* owner, const LPoint3f& init_pos, const LVector3f& init_dir, const float init_speed = 0.0f);
        // Initializes the Navigator with NULL route.
    bool Initialize(const string movtask_type, const LPoint3f& dest_pos = init_pos, const LVector3f& dest_vel = LVector3f(0.0f,0.0f,0.0f) )

    bool Move(const float dt);
        // Moves the WorldActor.
    bool Stop();
        // Stops the WorldActor from moving.

    // Inline Getters: 
    LPoint3f  pos()   { return pos_; }
    LVector3f dir()   { return dir_; }
    float     speed() { return speed_; }
    LVector3f up()    { return planet_->normal_at(pos_); }
    // LOL.
    Colorf speed_based_color() { return speed_based_color_; }

  private:

    /*** PRIVATE METHODS ***/

    bool TraceNewRoute(const LPoint3f& init_pos, const float init_vel, const LVector3f& init_dir,
                       const LPoint3f& dest_pos, const LVector3f& dest_vel);
        // Builds the nurbsCurve that represents the curve and returns true.
        // Returns a false if unsucsessful (i.e. if the curve is invalidated before creation).
    bool CreateMovTask(const string movtask_type);
        // Creates the movement task for the WorldActor and returns true if succesful.
    bool ValidateCurveBeforeCreation();
        // Reads the data that will be used to create a curve and preemtively unvalidates it.
    bool ValidateCurveAfterCreation();
        // Same as above, but used after the curve has been created.
        // In particular, returns false and throws a puts in the console if the curve doesn't yet exist.



    /*** ATTRIBUTES ***/

    //~ Whose WorldActor's Navigator Is This Anyway?
    WorldActor* _actor;

    //~ World state.
    Planet* planet_;

    //~ Current position and speed state.
    LPoint3f pos_;
    LVector3f dir_;
    float speed_;
    float current_param_;
        // The current curve parameter where the ship is located
        // (may be =0.0f if there is no curve).

    //~ Destination state.
    LPoint3f dest_pos_;
    LVector3f dest_vel_;

    //~ Route state.
    NurbsCurve* route_curve_;
        // The current stored curve.
    float curve_length_;
        // The route's length.

    //~ LOL. state.
    Colorf speed_based_color_;

};

}
}
}

#endif

