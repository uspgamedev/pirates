
#include "world/arrow.h"
#include "base/game.h"
#include "world/utils/routetracer.h"
#include "pandaFramework.h"
#include "routetracer.h"

static float knotvector[] = {0,0,0,0,4,4,4,4};
    // The only valid knot vector for a 4th degree NURBS with 4 control points and with parameter space = [0,4]

namespace pirates {

namespace world {

namespace utils {

using base::Game;

RouteTracer::RouteTracer( LPoint3f& init_pos, float init_vel, LVector3f& init_dir ) {
    LVector3f vectorial_vel( LVector3f(init_vel*init_dir) );
    route_curve_ = NULL;
    LPoint3f control_point = init_dir + 3*vectorial_vel; 
    trace_new_route( init_pos, init_vel, init_dir, control_point, vectorial_vel );
}

void RouteTracer::trace_new_route( LPoint3f& init_pos, float init_vel, LVector3f& init_dir, LPoint3f& dest_pos ) {
    // (with a point as destination)
    // TODO : test
    // TODO : reescrever isso, de forma que o barco faça uma rota mais esperta.
    if ( init_vel == 0.0f ) init_vel = 1.0f;

    LVector3f init_vect_vel(init_vel*init_dir);
    LVector3f shortest_path(dest_pos-init_pos);
    // rage mode on: COMO ASSIM NÃO TEM UM OPERADOR * ENTRE DOIS LVector3f ???????
    LVector3f dest_vel = 2*init_vect_vel.project(shortest_path) - init_vect_vel;

    trace_new_route(init_pos, init_vel, init_dir, dest_pos, dest_vel);
}

void RouteTracer::trace_new_route( LPoint3f& init_pos, float init_vel, LVector3f& init_dir, LPoint3f& dest_pos, LVector3f& dest_vel ) {
    // (with a point + vector as destination)

    if( init_vel == 0.0f ) init_vel = 1.0f;

    LVector3f init_vectorial_vel(init_dir*init_vel);

    // Adding the weights to the 3D vectors:
    LPoint4f  init_pos_4d(           init_pos.get_x(),           init_pos.get_y(),           init_pos.get_z(), 1.0f );
    LVector4f init_vel_4d( init_vectorial_vel.get_x(), init_vectorial_vel.get_y(), init_vectorial_vel.get_z(), 0.0f );
    LPoint4f  dest_pos_4d(           dest_pos.get_x(),           dest_pos.get_y(),           dest_pos.get_z(), 1.0f );
    LVector4f dest_vel_4d(           dest_vel.get_x(),           dest_vel.get_y(),           dest_vel.get_z(), 0.0f );

    // Building the control points vector and curve.
    LPoint4f cv_vector[] = {init_pos_4d, init_pos_4d + init_vel_4d, dest_pos_4d - dest_vel_4d, dest_pos_4d};

    if(route_curve_) delete route_curve_;
    route_curve_ = new NurbsCurve(4, 4, knotvector, cv_vector);

    // Setting up the route's default state.
    current_param_ = 0.0f;
    max_param_ = route_curve_->get_max_t();
    curve_length_ = route_curve_->calc_length();
}

void RouteTracer::get_next_pt( float vel, float dt, LPoint3f& cur_pos_ref, LVector3f& cur_tg_ref ) {

    // if vel*dt == 0 then there's nothing to do. cur_pos and cur_tg should be kept the same.
    // the route may not even be traced.
    if(vel == 0 || dt == 0)
        return;

    float param_ran = vel*dt/curve_length_;
    float new_param = current_param_ + param_ran;
        // If this calculation changes, remember to adjust the "temp_vel_ = vel;" line below.

    if( new_param >= max_param_ ) {
        // Curve ended. Trace new straight line and calculate the movement as if the curve and line were stitched.
        param_ran = param_ran - (max_param_ - current_param_);

        LPoint3f temp_startpoint = route_curve_->get_cv_point(3);
        float temp_vel = vel;
            // the correct way would be to calculate it from cv_point(3) - cv_point(2), but this forces it
            // to keep the same velocity it was using in the end of the movement.
        LVector3f temp_dir = temp_startpoint - route_curve_->get_cv_point(2);
        temp_dir = temp_dir/temp_dir.length();
        LVector3f temp_last_vect_vel = temp_vel*temp_dir;
        LPoint3f temp_lastpoint = temp_startpoint + 3*temp_last_vect_vel;
            // Control points are positioned with equal length between them.

        trace_new_route(temp_startpoint, temp_vel, temp_dir, temp_lastpoint, temp_last_vect_vel);
        current_param_ = param_ran;
    }
    else
        current_param_ = new_param;

    route_curve_->get_pt(current_param_, cur_pos_ref, cur_tg_ref);
}





} // namespace utils

} // namespace world

} // namespace pirates
