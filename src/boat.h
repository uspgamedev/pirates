////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: boat.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BOAT_H_
#define BOAT_H_

#include "pandaFramework.h"
#include <string>

namespace pirates {

class Boat {
    
  public :
   
    Boat (std::string &modelpath, WindowFramework*& window, PandaFramework& framework );
    
  private :

    NodePath boat_node_;
};

}
#endif
