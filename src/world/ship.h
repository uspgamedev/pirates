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

    LVector3f dir;

    LVector3f vel;

    LPoint3f node_pos() { return ship_node_.get_pos(); }

    AsyncTask::DoneStatus moveShip(GenericAsyncTask* task);

    void taskInitialize(AsyncTaskManager& taskMgr);

    utils::RouteTracer* route_tracer_;
    
  private :

    NodePath ship_node_;

    LPoint3f new_point;

};

} // namespace world
} // namespace pirates
#endif
