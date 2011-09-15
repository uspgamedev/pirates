////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: ship.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PIRATES_WORLD_SHIP_H_
#define PIRATES_WORLD_SHIP_H_

#include "lpoint3.h"
#include "asyncTask.h"
#include "genericAsyncTask.h"
#include "textureStage.h"
#include "pandaFramework.h"
#include "world/worldactor.h"

namespace pirates {

namespace world {

namespace utils {
class Navigator;
}

class Ship : public WorldActor {
    
  public :
    enum TraceMethod {
        DONT_TRACE = 0,
        DEST_ONLY = 1,
        DEST_AND_SPEED = 2
    };
   
    Ship();

    LPoint3f node_pos() { return ship_node_.get_pos(); }

    AsyncTask::DoneStatus moveShip(GenericAsyncTask* task);

    void taskInitialize(AsyncTaskManager& taskMgr);

    void set_new_route_dest(LPoint3f& dest);

    void set_new_route_dest(LPoint3f& dest_pos, LVector3f& dest_vel);
    
    bool anchored_;

    float matiz_;
    
    // New Stuff //

    TextureStage* texture_blend_stage() { return texture_blend_stage_; }


  private :

    NodePath ship_node_;

    utils::Navigator* navigator_;

    LPoint3f new_point;

    LVector3f new_tangent;

    int new_route_method_;

    LPoint3f new_route_dest_pos_;
    LVector3f new_route_dest_vel_;

    TextureStage* texture_blend_stage_;

};

} // namespace world
} // namespace pirates
#endif
