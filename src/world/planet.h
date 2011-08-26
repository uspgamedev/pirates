////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: planet.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PIRATES_WORLD_WORLD_H_
#define PIRATES_WORLD_WORLD_H_

#include "lpoint3.h"
#include "pandaFramework.h"
#include "world/worldactor.h"

namespace pirates {

namespace world {

class Planet : public WorldActor {
    
  public :
   
    Planet();

    LVector3f dir;

    LPoint3f node_pos() { return planet_node_.get_pos(); }

    AsyncTask::DoneStatus movePlanet(GenericAsyncTask* task);

    void taskInitialize(AsyncTaskManager& taskMgr);
    
  private :

    NodePath planet_node_;

};

} // namespace world
} // namespace pirates
#endif
