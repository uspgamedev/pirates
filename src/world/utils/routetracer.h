
#ifndef PIRATES_WORLD_UTILS_ROUTETRACER_H_
#define PIRATES_WORLD_UTILS_ROUTETRACER_H_

#include "NurbsCurve.h"
#include "ropeNode.h"

namespace pirates {
namespace world {
namespace utils {

class RouteTracer {

    public:
        RouteTracer(LPoint3f init_pos, LVector3f init_vel, LVector3f init_dir_if_stopped);
            // Initializes the RouteTracer and builds a default route (either stopped or straight with constant speed).
        void trace_new_route(LPoint3f init_pos, LVector3f init_vel, LVector3f init_dir_if_stopped, LPoint3f dest_pos, LVector3f dest_vel);
            // Builds the nurbsCurve that represents the path.
        LPoint3f/*void*/ get_next_point(float vel, float dt/*, LPoint3f& boat_cur_pos_ptr, LVector3f& boat_cur_tangent_ptr*/);
            // Returns the next point, based on the nurbsCurve, velocity of the boat and time elapsed.
            // If the end of the curve is reached, it should trace a new, straight curve.

    //private:
        PT(NurbsCurve) route_curve_;
            // The current stored curve.

    private:
        float current_param_;
            // The current stored parameter of the curve.
        float max_param_;
            // The stored curve's max value of the parameter.
        float curve_length_;
            // The curve's length.


};

}
}
}

#endif

