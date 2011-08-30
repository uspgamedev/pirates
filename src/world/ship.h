////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: ship.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PIRATES_WORLD_SHIP_H_
#define PIRATES_WORLD_SHIP_H_

#include "lpoint3.h"
#include "pandaFramework.h"
#include "world/worldactor.h"
#include "world/utils/routetracer.h"

namespace pirates {

namespace world {

class Ship : public WorldActor {
    
  public :
   
    Ship();

    LVector3f vel;

    LPoint3f node_pos() { return ship_node_.get_pos(); }

    AsyncTask::DoneStatus moveShip(GenericAsyncTask* task);

    void taskInitialize(AsyncTaskManager& taskMgr);

    utils::RouteTracer* route_tracer_;

    void set_new_route_dest(LPoint3f& dest);

    void set_new_route_dest(LPoint3f& dest_pos, LVector3f& dest_vel);
    
    bool anchored_;

    float matiz_;
    
  private :

    NodePath ship_node_;

    LPoint3f new_point;

    LVector3f new_tangent;

    int new_route_method_; // 0=no new route, 1=new route with dest only, 2=new route with dest and vel.

    LPoint3f new_route_dest_pos_;
    LVector3f new_route_dest_vel_;

};

} // namespace world
} // namespace pirates
#endif
