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

    LPoint3f node_pos() { return planet_node_.get_pos(); }

    const LVector3f normal_at(LPoint3f& pos);
    const float height_at(LPoint3f& pos);

  private :
    NodePath planet_node_;
};

} // namespace world
} // namespace pirates
#endif
