
#include "world/worldactor.h"

namespace pirates {
namespace world {

WorldActor::WorldActor(string name, NodePath node)
  : name_(name), node_(node), navigator_(NULL), texture_blend_stage_(NULL) {}



} // namespace world
} // namespace pirates

