  /************/
  /* INCLUDES */
  /************/

#include "world/utils/navigator.h"
#include "base/game.h" // used for GAME()->planet() .

  /*************/
  /* CONSTANTS */
  /*************/

#define MAX_PARAM 2
#define MID_PARAM 1
static float knotvector[] = {0, 0, 0, MID_PARAM, MAX_PARAM, MAX_PARAM, MAX_PARAM};
    // The only valid knot vector for a 3rd degree 'homogeneous' NURBS with 4 control points
    // and with parameter space = [0,MAX_PARAM] (MAX_PARAM effective 3-control-point knot spans).

  /**************/
  /* NAMESPACES */
  /**************/

namespace pirates {

namespace world {

namespace utils {

  /******************/
  /* MOVEMENT TASKS */
  /******************/

  /*********************/
  /* HueCyclingMovTask */
  /*********************/

HueCyclingMovTask::HueCyclingMovTask(const string movtask_name, const WorldActor* actor)
  : AsyncTask(movtask_name), actor_(actor), last_time_(0.0), planet_(GAME()->planet()),
  warned_once_about_texture_blend_stage_missing_(false) {}


AsyncTask::DoneStatus HueCyclingMovTask::do_task() {
    // Timing stuff.
    float dt = (float)( get_elapsed_time() - last_time_ );
    last_time_ = get_elapsed_time();

    // Find the WorldActor's Navigator and Node
    Navigator* navi = actor_->navigator();
    PandaNode* node = actor_->node();

    // Make the actor move.
    if(navi) {
        bool did_move = navi->Move(dt);
        if(did_move && node) {
            node->set_pos( navi->pos() );
            LPoint3f look_at = navi->pos() + navi->dir();
            node->look_at( look_at, navi->up() );
        }

        // LOL.
        TextureStage* ts_blend = actor_->texture_blend_stage();
        if(ts_blend)
            ts_blend->set_color( navi->speed_based_color() );
        else if(!warned_once_about_texture_blend_stage_missing_) {
            puts("HueCyclingMovTask::do_task() Warning:");
            puts("  a HueCyclingMovTask has been created for an WorldActor");
            puts("  that has no texture_blend_stage_ .\n");
            warned_once_about_texture_blend_stage_missing_ = true;
        }
    } else {
        // In this case, the parent Navigator has died.
        // This task is abnormal and should commit seppuku in a horrible, messy way.
        _manager->remove(this);
        return AsyncTask::DS_exit;
    }

    // And... We're done.
    return AsyncTask::DS_cont;
}

void HueCyclingMovTask::upon_death(AsyncTaskManager *manager, bool clean_exit) {

    puts("HueCyclingMovTask::upon_death\n");
    if(clean_exit)
        puts("  Task killed by returning DS_done (clean exit).\n");
    else {
        puts("  A movement task has died horribly...");
        puts("  (unclean exit, possibly due to its Navigator dying).\n");
    }

}


  /**************/
  /* NAVIGATORS */
  /**************/

Navigator::Navigator(const WorldActor* owner, const LPoint3f& init_pos,
                     const LVector3f& init_dir, const float init_speed = 0.0f)
  : _actor(owner), pos_(init_pos), dir_(init_dir), speed_(init_speed), dest_pos_(NULL), dest_vel_(NULL),
  current_param_(0.0f), route_curve_(NULL), curve_length_(0.0f), planet_(GAME()->planet()) {}

Navigator::Initialize(const string movtask_type, const LPoint3f& dest_pos = init_pos,
                      const LVector3f& dest_vel = LVector3f(0.0f,0.0f,0.0f)) {

    // Build the MovTask's name and create it.
    string movtask_name = _actor->name().substr();
           movtask_name+= "'s ";
           movtask_name+= movtask_type;
    bool did_create_task = CreateMovTask(movtask_type, movtask_name);



    return did_create_task;
}

/*
void Navigator::trace_new_route( LPoint3f& init_pos, float init_vel, LVector3f& init_dir, LPoint3f& dest_pos ) {
    // (with a point as destination)
    // TODO : reescrever isso, de forma que o barco faça uma rota mais esperta.
    if ( init_vel == 0.0f ) init_vel = 0.0001f;

    LVector3f init_vect_vel(init_vel*init_dir);
    LVector3f shortest_path(dest_pos-init_pos);
    LVector3f dest_vel = 2*init_vect_vel.project(shortest_path) - init_vect_vel;

    trace_new_route(init_pos, init_vel, init_dir, dest_pos, dest_vel);
}
*/

/*
void Navigator::trace_new_route( LPoint3f& init_pos, float init_vel, LVector3f& init_dir, LPoint3f& dest_pos, LVector3f& dest_vel = LVector3f(0,0,0) ) {
    // TODO: caso onde a vel final é nula ou mto próxima de 0.

    if( init_vel == 0.0f ) init_vel = 0.0001f;

    LVector3f init_vectorial_vel(init_dir*init_vel);

    // Adding the weights to the 3D vectors:
    LPoint4f  init_pos_4d(           init_pos.get_x(),           init_pos.get_y(),           init_pos.get_z(), 1.0f );
    LVector4f init_vel_4d( init_vectorial_vel.get_x(), init_vectorial_vel.get_y(), init_vectorial_vel.get_z(), 0.0f );
    LPoint4f  dest_pos_4d(           dest_pos.get_x(),           dest_pos.get_y(),           dest_pos.get_z(), 1.0f );
    LVector4f dest_vel_4d(           dest_vel.get_x(),           dest_vel.get_y(),           dest_vel.get_z(), 0.0f );

    // Building the control points vector and curve.
    LPoint4f cv_vector[] = {init_pos_4d, init_pos_4d + init_vel_4d, dest_pos_4d - dest_vel_4d, dest_pos_4d};

    if(route_curve_) delete route_curve_;
    route_curve_ = new NurbsCurve(3, 4, knotvector, cv_vector);

    // Setting up the route's default state.
    current_param_ = 0.0f;
    max_param_ = route_curve_->get_max_t();
    curve_length_ = route_curve_->calc_length();
}
*/

/*
void Navigator::get_next_pt( float vel, float dt, LPoint3f& cur_pos_ref, LVector3f& cur_tg_ref ) {

    // if vel*dt == 0 then there's nothing to do. cur_pos and cur_tg should be kept the same.
    if(vel <= 0.00001f || dt <= 0.00001f ) {
        puts("boat is stopped");
        return;
    }

    float dist_ran = vel*dt;
    float max_dist = route_curve_->calc_length(current_param_,max_param_);

    if( dist_ran >= max_dist ) {
        // Curve ended. Trace new straight line and calculate the movement as if the curve and line were stitched.
        dist_ran = dist_ran - max_dist;

        LPoint3f temp_startpoint = route_curve_->get_cv_point(3);

        LVector3f temp_dir = temp_startpoint - route_curve_->get_cv_point(2);
        temp_dir = temp_dir/temp_dir.length();
        LVector3f temp_last_vect_vel = vel*temp_dir;
        LPoint3f temp_lastpoint = temp_startpoint + 3*temp_last_vect_vel;
            // Control points are positioned with equal length between them.

        trace_new_route(temp_startpoint, vel, temp_dir, temp_lastpoint, temp_last_vect_vel);
    }
    current_param_ = route_curve_->find_length(current_param_,dist_ran);
    route_curve_->get_pt(current_param_, cur_pos_ref, cur_tg_ref);
}
*/





} // namespace utils

} // namespace world

} // namespace pirates
