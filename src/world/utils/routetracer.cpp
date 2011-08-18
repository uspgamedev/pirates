
#include "world/arrow.h"
#include "base/game.h"
#include "world/utils/routetracer.h"
#include "pandaFramework.h"
#include "routetracer.h"

static float knotvector[] = {0,0,0,1,2,2,2};

namespace pirates {

namespace world {

namespace utils {

using base::Game;

RouteTracer::RouteTracer(LPoint3f init_pos, LVector3f init_vel, LVector3f init_dir_if_stopped) {
    puts("construtor do routetracer chamado");
    trace_new_route(init_pos, init_vel, init_dir_if_stopped, init_pos + init_vel, init_vel);
}

void RouteTracer::trace_new_route(LPoint3f init_pos, LVector3f init_vel, LVector3f init_dir_if_stopped, LPoint3f dest_pos, LVector3f dest_vel) {
    puts("traçando nova rota");
    // adding the weights to the 3D vectors:
    LPoint4f init_pos_4d, dest_pos_4d;
    LVector4f init_vel_4d, init_dir_if_stopped_4d, dest_vel_4d;

    init_pos_4d.set(init_pos.get_x(), init_pos.get_y(), init_pos.get_z(), 1.0f);
    dest_pos_4d.set(dest_pos.get_x(), dest_pos.get_y(), dest_pos.get_z(), 1.0f);

    init_vel_4d.set(init_vel.get_x(), init_vel.get_y(), init_vel.get_z(), 0.0f);
    init_dir_if_stopped_4d.set(init_dir_if_stopped.get_x(), init_dir_if_stopped.get_y(), init_dir_if_stopped.get_z(), 0.0f);
    dest_vel_4d.set(dest_vel.get_x(), dest_vel.get_y(), dest_vel.get_z(), 0.0f);

    LPoint4f cv_vector[] = {init_pos_4d, init_pos_4d + init_vel_4d, dest_pos_4d - dest_vel_4d, dest_pos_4d};

    current_param_ = 0.0f;
    route_curve_ = new NurbsCurve(3, 4, knotvector, cv_vector);
    max_param_ = route_curve_->get_max_t();
    curve_length_ = route_curve_->calc_length();
    printf("nova rota traçada. max_param_ = %f, curve_length_ = %f\n", max_param_, curve_length_);
    printf("control point info: in_pos:(%f,%f,%f),dest_pos:(%f,%f,%f),\nin_vel:(%f,%f,%f),dest_vel:(%f,%f,%f)\n", init_pos.get_x(), init_pos.get_y(), init_pos.get_z(), dest_pos.get_x(), dest_pos.get_y(), dest_pos.get_z(), init_vel.get_x(), init_vel.get_y(), init_vel.get_z(), dest_vel.get_x(), dest_vel.get_y(), dest_vel.get_z());
    printf("actual control points: 0-(%f,%f,%f),1-(%f,%f,%f),\n2-(%f,%f,%f),3-(%f,%f,%f)\n\n", route_curve_->get_cv_point(0).get_x(), route_curve_->get_cv_point(0).get_y(), route_curve_->get_cv_point(0).get_z(), route_curve_->get_cv_point(1).get_x(), route_curve_->get_cv_point(1).get_y(), route_curve_->get_cv_point(1).get_z(), route_curve_->get_cv_point(2).get_x(), route_curve_->get_cv_point(2).get_y(), route_curve_->get_cv_point(2).get_z(), route_curve_->get_cv_point(3).get_x(), route_curve_->get_cv_point(3).get_y(), route_curve_->get_cv_point(3).get_z());
}

LPoint3f /*void*/ RouteTracer::get_next_point(float vel, float dt/*, LPoint3f& boat_cur_pos_ref, LVector3f& boat_cur_tangent_ref */) {
    float dist_ran = vel*dt;
    float new_param = route_curve_->find_length(current_param_, dist_ran);

     if( new_param >= max_param_ ) {
        puts("curva terminou! criando nova curva");
        // Curve ended. Trace new straight line and calculate the movement as if the curve and line were stitched.
        dist_ran = dist_ran - route_curve_->calc_length(current_param_, max_param_);

        LPoint3f new_startpoint = route_curve_->get_cv_point(3);
        LVector3f new_vel = new_startpoint - route_curve_->get_cv_point(2);
        LPoint3f new_lastpoint = new_startpoint + 2*new_vel;

        trace_new_route(new_startpoint, new_vel, new_vel, new_lastpoint, new_vel);
        current_param_ = route_curve_->find_length(0, dist_ran);
    }
    else
        current_param_ = new_param;

    LPoint3f& boat_cur_pos_ref(LPoint3f(6,7,8));

    route_curve_->get_point(current_param_, boat_cur_pos_ref);
    return boat_cur_pos_ref;
}





} // namespace utils

} // namespace world

} // namespace pirates