
#include "boat.h"

namespace pirates {

Boat::Boat (std::string &modelpath, WindowFramework*& window, PandaFramework& framework ) {
    NodePath m = window->load_model(framework.get_models(), modelpath);
    m.reparent_to(window->get_render());
    m.set_pos(0, 0, 0);
    boat_node_ = m;
}

}
