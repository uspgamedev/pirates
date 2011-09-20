
  /************/
  /* INCLUDES */
  /************/

#include "world/utils/navigator.h"
#include "world/worldactor.h"
#include "world/utils/movtasks.h"
#include "base/game.h" // used for GAME()->planet().
#include <memory>

  /*************/
  /* CONSTANTS */
  /*************/

#define DEST_IS_ORIGIN_THRESHOLD 0.1f
#define MAX_SQUARED_DISTANCE_BEFORE_CURVE_RECALC 64.0f

#define MIN_SPEED 0.5f
#define SPEED_IS_ZERO_THRESHOLD 0.1f
#define VEL_IS_ZERO_THRESHOLD 0.2f

#define SPEED_PENALTY_FROM_CURVATURE_SCALAR 10.0f
#define ACTOR_BRAKING_FACTOR 50.0f

#define MAX_PARAM 2.0f
#define MID_PARAM 1.0f
const static float knotvector[7] = {0.0f, 0.0f, 0.0f, MID_PARAM, MAX_PARAM, MAX_PARAM, MAX_PARAM};
    // The only valid knot vector for a 3rd degree 'homogeneous' NURBS with 4 control points
    // and with parameter space = [0,MAX_PARAM] (MAX_PARAM effective 3-control-point knot spans).

  /**************/
  /* NAMESPACES */
  /**************/

namespace pirates {
namespace world {

namespace utils {

  /*************/
  /* NAVIGATOR */
  /*************/

Navigator::Navigator(WorldActor* owner, const LPoint3f& init_pos, const LVector3f& init_dir)
  : initialized_(false), current_movtask_(NULL), actor_(owner), pos_(init_pos), dir_(init_dir), speed_(0.0f),
  current_param_(0.0f), stopping_(true), route_curve_(NULL), curve_length_(0.0f), planet_(GAME()->planet()) {
}

bool Navigator::Initialize(const MovTask_Types movtask_type, const float speed,
                           const LPoint3f& dest_pos, const LVector3f& dest_vel) {

    puts("Initializing Navigator");
    if( speed >= SPEED_IS_ZERO_THRESHOLD || dest_pos.compare_to(pos_, DEST_IS_ORIGIN_THRESHOLD) != 0 )
        Move();

    if( !TraceNewRouteTo(dest_pos, dest_vel) )
        Stop();
        //waypoint_list_.push_front( Waypoint(dest_pos, dest_vel) ); //TODO
    else {
        route_curve_->get_2ndtangent(0.0f, current_tangent_);
        dir_ = current_tangent_;
        dir_.normalize();
    }

    // CreateMovTask takes care of setting initialized_ to true.
    return CreateMovTask(movtask_type);
}

bool Navigator::Step(const float dt) {
    
    // Better safe than sorry.
    if( dt == 0 )
        return true;

    // Find the new speed and old direction.
    if( route_curve_ ) { // We'll treat this possible error later, since we need to do some other stuf before.
        dir_ = current_tangent_;
        dir_.normalize();
        route_curve_->get_tangent(current_param_, current_tangent_);

        speed_ = current_tangent_.length();

        if( !stopping_ ) {
            LVector3f tangent_direction_change = current_tangent_/speed_ - dir_;
            float speed_penalty = tangent_direction_change.length()*SPEED_PENALTY_FROM_CURVATURE_SCALAR;

            speed_ -= speed_penalty; // We'll check if this is negative very soon.
        }
    }

    // Test if there's a problem with the new speed.
    if( speed_ < MIN_SPEED ) {
        if( !stopping_ )   // TraceRoute would've set this higher if the actor is now trying to move,
            Stop();        // so the actor tried to make a movement he can't, and we should order it to stop now.
        return false;
    }

    // Find the distance to run.
    float dist_ran = speed_*dt;

    // If there's no curve, well... WTF happened?
    if( !route_curve_ ) {
        if( stopping_ ) {
            fprintf(stderr, "Warning from Navigator::Step(%f) :\n", dt);
            fprintf(stderr, "    WorldActor \"%s\"'s Navigator is stopping_,\n", actor_->name() );
            fprintf(stderr, "    has no route_curve_, but has speed_ == %f >= SPEED_IS_ZERO_THRESHOLD.\n", speed_);
            fprintf(stderr, "    Maybe the threshold is too big? Freezing the WorldActor in place anyway...\n");
            speed_ = 0.0f;
            return false;
        }
        fprintf(stderr, "Error from Navigator::Step(%f) :\n", dt);
        fprintf(stderr, "    WorldActor \"%s\"'s Navigator\n", actor_->name() );
        fprintf(stderr, "    is not stopping_, but has no route_curve_.\n");
        // The route despawned..? Weird... build it and try again then.
        if( waypoint_list_.empty() ) {
            if( !Move() )
                Stop();
            return Step(dt);
        }
        if( !TraceRoute() )
            Stop();
        return Step(dt);
    }

    // Time to do the magic.
    float max_distance = route_curve_->calc_length(current_param_,MAX_PARAM);
    if( dist_ran >= max_distance ) {
        // Then it means the curve ended. Discard the waypoint, build the next route, update parameter.
        waypoint_list_.pop_front();
        if( waypoint_list_.empty() ) {
            if( !stopping_ ) {
                if( !Move() ) // updates the waypoint list with default ones.
                    Stop();
            } else             
                Stop(); // same thing, but this will eventually stop the ship.
        }
        TraceRoute();
        dist_ran -= max_distance;
        current_param_ = 0.0f;
    }

    // Get the new virtual position. We still need to project it onto the planet.
    current_param_ = route_curve_->find_length(current_param_,dist_ran);
    route_curve_->get_point(current_param_, pos_);

    // Get the first point of the current route curve, and planet center, used for the projection.
    LPoint4f first_cv = route_curve_->get_cv(0);
    const LPoint3f& planet_center = planet_->center();
    
    // Project the virtual position onto the planet, making it a real one. 
    //TODO: better projection
    pos_ -= planet_center;
    pos_.normalize();
    pos_ *= planet_->height_at(pos_);
    pos_ += planet_center;

    // Update the curve if we're too far away from its origin.
/*DEBUGGING    if( (LVector3f(pos_.get_x() - first_cv.get_x(), pos_.get_y() - first_cv.get_y(),
         pos_.get_z() - first_cv.get_z())).length_squared() >= MAX_SQUARED_DISTANCE_BEFORE_CURVE_RECALC )
        TraceRoute();
*/        
    return true;
}

bool Navigator::Stop() {
    puts("Stopping...");
    if( !stopping_ ) {
        stopping_ = true;/*DEBUGGING
        if( speed_ < SPEED_IS_ZERO_THRESHOLD ) {*/
            if( route_curve_ ) { delete route_curve_; route_curve_ = NULL; current_param_ = 0.0f; }
            speed_ = 0.0f;
            return true;/*DEBUGGING
        }
        float distance_to_run = speed_*speed_/ACTOR_BRAKING_FACTOR;
        LPoint3f straight_ahead = pos_ + (speed_ + distance_to_run)*dir_;
        LVector3f zero_speed = SPEED_IS_ZERO_THRESHOLD*0.5f*dir_;
        waypoint_list_.push_front( Waypoint(straight_ahead,zero_speed) );
        if( !TraceRoute() ) {
            actor_->DieFromOopsYoureInsideAWall();
            return false;
        }*/
    }
    return true;

}

bool Navigator::Move() {
    
    if( speed_ < MIN_SPEED )
        speed_ = MIN_SPEED;
    stopping_ = false;
    if( waypoint_list_.empty() ) {
        LPoint3f straight_ahead = pos_ + 3.0f*speed_*dir_;
        LVector3f vel = speed_*dir_;
        waypoint_list_.push_front( Waypoint(straight_ahead,vel) );
    }
    return true;
}

bool Navigator::TraceNewRouteTo(const LPoint3f& dest_pos, const LVector3f& dest_vel) {
    // We're dropping all waypoints.
    if(!waypoint_list_.empty()) waypoint_list_.clear();
    // Build the new waypoint and add it to the list.
    Waypoint waypoint = Waypoint(dest_pos, dest_vel);
    waypoint_list_.push_front( waypoint );
    // Decide if we should move or not.
    Move();
    if(!TraceRoute()) {
        if(!Stop()) {
            fprintf(stderr, "Warning: Navigator::TraceNewRouteTo((%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f))\n",
                    dest_pos.get_x(), dest_pos.get_y(), dest_pos.get_z(), dest_vel.get_x(), dest_vel.get_y(), dest_vel.get_z());
            fprintf(stderr, "  says: \"Oops, you're inside a wall.\"\n");
        }
        speed_ = 0.0f;
        stopping_ = true;
        return false;
    }
    return true;
}

bool Navigator::TraceRoute() {
    // Can't do much if there's nowhere to go. Get a reference to the destination otherwise.
    if( waypoint_list_.empty() ) {
        fprintf(stderr, "Warning: Navigator::TraceRoute() called with no destination set.\n");
        return false;
    }
    Waypoint& waypoint = waypoint_list_.front();

    // If the boat is not stopping/stopped, and is too slow, then it means it's trying to start moving.
    if(speed_ < SPEED_IS_ZERO_THRESHOLD && !stopping_)
        speed_ = MIN_SPEED;
    else if(speed_ < SPEED_IS_ZERO_THRESHOLD) {
        // Uh... you should stop the ship then. How did you get in here?
        fprintf(stderr, "Warning: Navigator::TraceRoute() called while stopping with no speed.\n");
        fprintf(stderr, "    Deleting current route curve (if exists) and setting the speed to 0.0f.\n");
        if(route_curve_) { delete route_curve_; route_curve_ = NULL; current_param_ = 0.0f; }
        speed_ = 0.0f;
        return false;
    }

    // Now, we'll need the initial velocity, not direction + speed.
    LVector3f vel = dir_*speed_;

    // If the player hasn't defined a vectorial velocity for the ship at destination, we must build it.
    if( waypoint.second.compare_to(LVector3f::zero(), VEL_IS_ZERO_THRESHOLD) == 0 ) {

        LPoint3f second_point = pos_ + vel;                 // Where the actor is looking at.
        waypoint.second = waypoint.first - second_point;    // Find the direction from where he's looking at,
        waypoint.second.normalize();                        //      to the destination
        waypoint.second*=speed_;                            // Keep the same speed at destination.
    }
    // Now, we can validate the route parameters.
    if( !ValidateRouteBeforeCreation() )
        return false;

    // Adding the weights to the 3D vectors:
    LPoint4f  init_pos_4d(            pos_.get_x(),            pos_.get_y(),            pos_.get_z(), 1.0f );
    LVector4f init_vel_4d(             vel.get_x(),             vel.get_y(),             vel.get_z(), 0.0f );
    LPoint4f  dest_pos_4d(  waypoint.first.get_x(),  waypoint.first.get_y(),  waypoint.first.get_z(), 1.0f );
    LVector4f dest_vel_4d( waypoint.second.get_x(), waypoint.second.get_y(), waypoint.second.get_z(), 0.0f );
    
    // Building the control points vector and curve.
    LPoint4f cv_vector[4] = {init_pos_4d, init_pos_4d + init_vel_4d, dest_pos_4d - dest_vel_4d, dest_pos_4d};

    if(route_curve_) { delete route_curve_; route_curve_ = NULL; current_param_ = 0.0f; }
    route_curve_ = new NurbsCurve(3, 4, knotvector, cv_vector);

    // And... we should validate the curve now.
    if( !ValidateRouteAfterCreation() )
        return false;

    // Setting up the route's default state.
    current_param_ = 0.0f;
    curve_length_ = route_curve_->calc_length();
    return true;
}

bool Navigator::CreateMovTask(const MovTask_Types movtask_type) {

    string movtask_name = actor_->name();
    movtask_name+= "'s";

    //TODO: FIX THIS

    movtask_name+= " hue cycling movtask";

    // the following does basically:
    // current_movtask_ = new HueCyclingMovTask(movtask_name, actor_);
    // but the former doesn't work for some strange reason, so we do:

    HueCyclingMovTask task(movtask_name, actor_);

    allocator<HueCyclingMovTask> allocator;
    HueCyclingMovTask* pointer = allocator.allocate(1);
    allocator.construct(pointer, task);

    current_movtask_ = pointer;
    //current_movtask_ = new HueCyclingMovTask(movtask_name, actor_);

    /*
    switch(movtask_type) {
        case DEFAULT_MOV:
            return false; //TODO: This will obviously be changed.
            break;
        case HUE_CYCLING_MOV:
            movtask_name+= " hue cycling movtask";

            HueCyclingMovTask task(movtask_name, actor_);

            allocator<HueCyclingMovTask> allocator;
            HueCyclingMovTask* pointer = allocator.allocate(1);
            allocator.construct(pointer, task);

            current_movtask_ = pointer;
            //current_movtask_ = new HueCyclingMovTask(movtask_name, actor_);
            //TODO: check if the task was created safely.
            break;
        default:
            break;
    }
    */
    GAME()->taskMgr().add(current_movtask_);
    initialized_ = true;
    return true;
}

bool Navigator::ValidateRouteBeforeCreation() {
    //TODO
    return true;
}

bool Navigator::ValidateRouteAfterCreation() {
    //TODO
    return true;
}

} // namespace utils

} // namespace world

} // namespace pirates