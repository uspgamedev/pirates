////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: boat.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BOAT_H_
#define BOAT_H_

#include "pandaFramework.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"
#include "lpoint3.h"
#include <string>

namespace pirates {

class Boat {
    
  public :
   
    Boat (std::string &modelpath, WindowFramework*& window, PandaFramework& framework );
    LPoint3f node_pos () { return boat_node_.get_pos(); }
    AsyncTask::DoneStatus moveBoat ( GenericAsyncTask* task, void* data );
    void taskInicialize();
    
  private :
    PT(AsyncTaskManager) taskMgr_;
    PT(ClockObject) globalClock_;
    NodePath boat_node_;
};

}
#endif
