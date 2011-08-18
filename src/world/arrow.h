
#ifndef PIRATES_WORLD_ARROW_H_
#define PIRATES_WORLD_ARROW_H_

#include "lpoint3.h"
#include "nodePath.h"
#include "world/worldactor.h"

namespace pirates {

namespace world {

class Arrow : public WorldActor {

  public:

    Arrow();

    void set_pos(LPoint3f pos);

    void look_at(LPoint3f target);

  private:

    NodePath arrow_node_;

};

}

}

#endif

