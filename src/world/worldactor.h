
#ifndef PIRATES_WORLD_WORLDACTOR_H_
#define PIRATES_WORLD_WORLDACTOR_H_

#include "pandaFramework.h"

namespace pirates {
namespace world {

namespace utils {
class Navigator;
}

class WorldActor /*: public WorldObject*/ {

  public:
    WorldActor(string name, NodePath node);

    bool DieFromOopsYoureInsideAWall() { return false; }

    const string& name() const { return name_; }
    NodePath* node() { return &node_; }

    utils::Navigator* navigator() { return navigator_; }
    TextureStage* texture_blend_stage() { return texture_blend_stage_; }

  protected:
    const string name_;
    NodePath node_;

    utils::Navigator* navigator_;
    TextureStage* texture_blend_stage_;

};

} // namespace world
} // namespace pirates

#endif

