////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: world.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PIRATES_WORLD_WORLD_H_
#define PIRATES_WORLD_WORLD_H_

#include "lpoint3.h"
#include "pandaFramework.h"
#include "world/worldactor.h"

namespace pirates {

namespace world {

class World : public WorldActor {
    
  public :
   
    World();

    LVector3f dir;

    LPoint3f node_pos() { return world_node_.get_pos(); }

    AsyncTask::DoneStatus moveWorld(GenericAsyncTask* task);

    void taskInitialize(AsyncTaskManager& taskMgr);
    
  private :

    NodePath world_node_;

};

} // namespace world
} // namespace pirates
#endif
