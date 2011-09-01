
#ifndef PIRATES_WORLD_UTILS_ROUTETRACER_H_
#define PIRATES_WORLD_UTILS_ROUTETRACER_H_

#include "nurbsCurve.h"
#include "ropeNode.h"

namespace pirates {
namespace world {
namespace utils {

class Navigator {

    public:
        Navigator(LPoint3f& init_pos, float init_vel, LVector3f& init_dir);
            // Initializes the Navigator and builds a default route (either stopped or straight with constant speed).

        void trace_new_route(LPoint3f& init_pos, float init_vel, LVector3f& init_dir, LPoint3f& dest_pos);
        void trace_new_route(LPoint3f& init_pos, float init_vel, LVector3f& init_dir, LPoint3f& dest_pos, LVector3f& dest_vel);
            // Builds the nurbsCurve that represents the path.

        void get_next_pt(float vel, float dt, LPoint3f& cur_pos_ptr, LVector3f& cur_tg_ptr);
            // Returns the next point, based on the nurbsCurve, velocity of the boat and time elapsed.
            // If the end of the curve is reached, it should trace a new, straight curve.

    private:
        NurbsCurve* route_curve_;
            // The current stored curve.

        float current_param_;
            // The current stored parameter of the curve where the ship is located.
        float max_param_;
            // The stored curve's max value of the parameter.
        float curve_length_;
            // The route's length.
};

}
}
}

#endif

