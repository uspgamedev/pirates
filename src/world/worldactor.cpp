
#include "world/worldactor.h"
#include "pandaFramework.h"
#include "nodePath.h"
#include "pnmImage.h"
#include "texture.h"

namespace pirates {
namespace world {

WorldActor::WorldActor(const string& name, const string& model)
  : name_(name), model_(model), initialized_(false), navigator_(NULL), texture_blend_stage_(NULL) {
}

const bool WorldActor::Initialize(const LPoint3f& init_pos, const LPoint3f& init_look_at, LVector3f init_up) {

    if(init_up.get_x() == 0.0f && init_up.get_y() == 0.0f && init_up.get_z() == 0.0f)
        init_up = GAME()->planet()->normal_at(init_pos);

    if(!initialized_) {
        PandaFramework& framework = GAME()->framework();
        node_ = GAME()->window()->load_model(framework.get_models(),model_);
        node_.set_scale(0.5f);
        node_.set_pos(init_pos);
        node_.look_at(init_look_at,init_up);
        node_.reparent_to(GAME()->window()->get_render());

        initialized_ = true;
        return true;
    }
    return false;
}

bool WorldActor::DieFromOopsYoureInsideAWall() {
    fprintf(stderr,"Oops... you're inside a wall...\n");
    return false; //TODO: implement this error
}

} // namespace world
} // namespace pirates

