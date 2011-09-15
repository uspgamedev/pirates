
#ifndef PIRATES_WORLD_UTILS_NAVIGATOR_H_
#define PIRATES_WORLD_UTILS_NAVIGATOR_H_

#include "pandaFramework.h"
#include "nurbsCurve.h"
#include "world/planet.h"
#include <string>
#include <utility>
#include <list>

namespace pirates {
namespace world {

class WorldActor;

namespace utils {

class Navigator {

  public:
    /*** PUBLIC CONSTANTS ***/

    typedef std::pair<LPoint3f, LVector3f> Waypoint;

    enum MovTask_Types {
        DEFAULT_MOV = 0,
        HUE_CYCLING_MOV = 1
    };

    /*** PUBLIC METHODS ***/

    //~ Constructor and Initializer:
    // Builds the Navigator with NULL route. Doesn't create the MovTask.
    Navigator(WorldActor* owner, const LPoint3f& init_pos, const LVector3f& init_dir);
    // Initializes the Navigator, with possible startup speed/destination.
    // This builds the Navigator, MovTask, and default route (may be NULL).
    bool Initialize(const MovTask_Types movtask_type, const float init_speed,
                    const LPoint3f& dest_pos, const LVector3f& dest_vel = LVector3f(0.0f) );

    bool Initialize(const MovTask_Types movtask_type, const float init_speed = 0.0f ) {
        const LPoint3f& dest_pos = LPoint3f(pos());
        return Initialize(movtask_type, init_speed, dest_pos);
    }
    bool Initialize(const MovTask_Types movtask_type, const LPoint3f& dest_pos, const LVector3f& dest_vel = LVector3f(0.0f) ) {
        return Initialize(movtask_type, 0.0f, dest_pos, dest_vel);
    }

    //~ Standard Navigator Public Methods:

    bool Step(const float dt);
        // Moves the WorldActor one step. This function is called by the Navigator's associated MovTask.
    bool Stop();
        // Stops the WorldActor from moving.
    bool Move();
        // Stops the WorldActor from being stopped.
    bool TraceNewRouteTo(const LPoint3f& dest_pos, const LVector3f& dest_vel = LVector3f(0.0f));
        // Clears the waypoint list, adds the waypoint built from the parameters to the list, traces the route.
    bool AddWaypoint(const Waypoint waypoint);
        // Adds a waypoint to the queue and tests if it seems valid.

    //~ Inline Getters.
    const LPoint3f&  pos()   const { return pos_;   }
    const LVector3f& dir()   const { return dir_;   }
    const float      speed() const { return speed_; }
    const std::list<Waypoint>& waypoint_list() const { return waypoint_list_; }
    //~ Semi-Getters.
    /* cant return NULL =(.
    const LPoint3f&  dest_pos() const { if(waypoint_list_.size() > 0) return waypoint_list_.front().first;  return NULL; }
    const LVector3f& dest_vel() const { if(waypoint_list_.size() > 0) return waypoint_list_.front().second; return NULL; }
    */
    LVector3f vel() const { return dir_*speed_; }
    LVector3f  up() const { return planet_->normal_at(pos_); }



  private:
    /*** PRIVATE METHODS ***/

    bool TraceRoute();
        // Traces the new route, reading the next waypoint.
    bool CreateMovTask(const MovTask_Types movtask_type);
        // Creates the movement task for the WorldActor and returns true if succesful.
    bool ValidateRouteBeforeCreation();
        // Reads the data that will be used to create a curve and preemtively unvalidates it.
    bool ValidateRouteAfterCreation();
        // Same as above, but used after the curve has been created.
        // In particular, returns false and throws a puts in the console if the curve doesn't yet exist.


    /*** ATTRIBUTES ***/

    //~ We should Initialize only once...
    bool initialized_;
    //~ And we should know the task, as we might want to kill it.
    AsyncTask* current_movtask_;
    //~ Whose WorldActor's Navigator Is This Anyway?
    WorldActor* actor_;
    //~ Where are we?.
    const Planet* planet_;

    //~ Current position and speed state.
    LPoint3f pos_;
    LVector3f dir_;
        // Actually, the direction from last Step() frame.
    float speed_;
    float current_param_;
        // The current curve parameter where the ship is located
        // (may be =0.0f if there is no curve).
    bool stopping_; // Also covers the case "stopped and not starting to move".
    // The following are used only by Step().
    LVector3f current_tangent_;
        // The route's tangent at current_param_.

    //~ Destination and waypoints state.
    std::list<Waypoint> waypoint_list_;

    //~ Route state.
    NurbsCurve* route_curve_;
        // The current stored curve.
    float curve_length_;
        // The route's length.
};

}
}
}

#endif

