
#ifndef PIRATES_WORLD_WORLDACTOR_H_
#define PIRATES_WORLD_WORLDACTOR_H_

#include "lpoint3.h"
#include "lvector3.h"
#include "base/game.h"
#include "world/utils/navigator.h"
#include <string>

class NodePath;
class TextureStage;

namespace pirates {
namespace world {

class WorldActor /*: public WorldObject*/ {

  public:

    /*** CONSTRUCTOR, INITIALIZER and DESTRUCTOR ***/

    WorldActor(const string& name, const string& model = string("data/pure_worldactor") );
        //TODO: make the pure worldactor model.

    virtual const bool Initialize(const LPoint3f&   init_pos     = GAME()->planet()->default_start_pos(),
                                  const LPoint3f&   init_look_at = GAME()->planet()->default_look_at(),
                                  LVector3f        init_up      = LVector3f(0.0f)  ) = 0;


    /*** PUBLIC METHODS ***/

    // Fatal error called from utils::Navigator::Stop(),
    //     marks the fact that the WorldActor is unable to trace routes.
    bool DieFromOopsYoureInsideAWall();


    /*** INLINE PUBLIC METHODS ***/

    const LPoint3f pos() { if(navigator_) return navigator_->pos(); return node_.get_pos(); }

    const string& name() const { return name_; }
    const string& model () const { return model_; }
    NodePath& node() { return node_; }

    utils::Navigator* navigator() { return navigator_; }
    TextureStage* texture_blend_stage() { return texture_blend_stage_; }

  protected:

    /*** ATTRIBUTES ***/

    //~ Required for existence
    const string name_;  //TODO: move this to WorldObject
    const string model_; //TODO: move this to WorldObject
    bool initialized_;   //TODO: move this to WorldObject

    //~ Built on Initialization
    NodePath node_;
        // Also holds the WorldActor's position for immovable WorldActors.

    //~ Optional, depending on WorldActor's functionality and appearance.
    //~     users of these should check for their existence when needed.
    // optional funcionalities
    utils::Navigator* navigator_;
        // Needed for WorldActors capable of movement.
    // optional cosmetic appearance
    TextureStage* texture_blend_stage_;
        // Needed for WorldActors who use blend textures.

};

} // namespace world
} // namespace pirates

#endif
