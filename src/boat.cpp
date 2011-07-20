
#include "boat.h"

namespace pirates {

Boat::Boat (std::string &modelpath, WindowFramework*& window, PandaFramework& framework ) {
    NodePath m;
    m = window->load_model(framework.get_models(), "panda-model");
    m.set_scale(0.005);
    m.reparent_to(window->get_render());
    m.set_pos(5, 5, 5);
    boat_node_ = m;
}

}
