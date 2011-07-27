////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: boat.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BOAT_H_
#define BOAT_H_

#include "lpoint3.h"
#include "pandaFramework.h"

namespace pirates {

namespace base {

class Boat {
    
  public :
   
    Boat (std::string &modelpath, WindowFramework*& window, PandaFramework& framework );
    LPoint3f node_pos () { return boat_node_.get_pos(); }
    AsyncTask::DoneStatus moveBoat ( GenericAsyncTask* task );
    void taskInicialize( AsyncTaskManager& taskMgr );
    
  private :
    NodePath boat_node_;
};

} // namespace base
} // namespace pirates
#endif
