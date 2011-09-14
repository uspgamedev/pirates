////////////////////////////////////////////////////////////////////////////////////////////////////
// (temporary header comment)
// Project Pirates by USPGameDev
// Filename: planet.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PIRATES_WORLD_WORLD_H_
#define PIRATES_WORLD_WORLD_H_

#include "lpoint3.h"
#include "pandaFramework.h"
#include "collisionSolid.h"
#include "collisionSphere.h"

#include "world/worldactor.h"

namespace pirates {

namespace world {

class Planet : public WorldActor {    
  public :
    Planet();

    LPoint3f node_pos() { return planet_node_.get_pos(); }

    LVector3f normal_at(const LPoint3f& pos) const;
    const float height_at(const LPoint3f& pos) const;

    CollisionSolid* get_collision() { return collision_; }

  private :
    NodePath planet_node_;
    PT(CollisionSphere) collision_;
};

} // namespace world
} // namespace pirates
#endif
