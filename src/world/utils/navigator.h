
#ifndef PIRATES_WORLD_UTILS_ROUTETRACER_H_
#define PIRATES_WORLD_UTILS_ROUTETRACER_H_

#include "nurbsCurve.h"
#include "ropeNode.h"
#include "asyncTask.h"

namespace pirates {
namespace world {

class Ship;

namespace utils {

class Navigator;
class StandardShipMovTask : public AsyncTask {
  public:
    StandardShipMovTask(Ship* ship);
  protected:
    AsyncTask::DoneStatus do_task();
    void upon_death(AsyncTaskManager *manager, bool clean_exit);
  private:
    Ship* ship_;
    double last_time_;
};


class Navigator {

  public:
    Navigator(const LPoint3f& init_pos, const LVector3f& init_dir, const int );
        // Initializes the Navigator with NULL route, creates the right MoveTask.

    void trace_new_route(const LPoint3f& init_pos, const float init_vel,
                         const LVector3f& init_dir, const LPoint3f& dest_pos, const LVector3f& dest_vel);
        // Builds the nurbsCurve that represents the path.

    //void GetNextPointAndTangent(const float vel, const float dt, LPoint3f& cur_pos_ptr, LVector3f& cur_tg_ptr);
        // Returns the next point, based on the nurbsCurve, velocity of the boat and time elapsed.
        // If the end of the curve is reached, it should trace a new, straight curve.
        // DEPRECATED due to the new "Move" function and refactoring RouteTracer -> Navigator


    // New Stuff //

    bool CreateMovTask(const char* task_name);

    bool Anchor();

    bool Move(const float dt);

    // Inline Funcions: 
    LPoint3f pos() { return pos_; }
    LVector3f dir() { return dir_; }
    float speed() { return speed_; }
    LVector3f up() { return up_; }

  private:
    NurbsCurve* route_curve_;
        // The current stored curve.
    float current_param_;
        // The current stored parameter of the curve where the ship is located.
    float max_param_;
        // The stored curve's max value of the parameter.
    float curve_length_;
        // The route's length.


    // New Stuff //

    LPoint3f pos_;
    LVector3f dir_;
    float speed_;
    LVector3f up_;
    // LOL.
    Colorf speed_based_color_;

};

}
}
}

#endif

