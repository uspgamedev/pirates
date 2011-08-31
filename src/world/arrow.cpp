
#include "world/arrow.h"
#include "base/game.h"
#include "pandaFramework.h"

namespace pirates {

using base::Game;

namespace world {

Arrow::Arrow() {
    PandaFramework& framework = Game::reference()->framework();
    WindowFramework* window = Game::reference()->window();
    arrow_node_ = window->load_model(framework.get_models(), "data/old_king");
    arrow_node_.set_scale(5);
    arrow_node_.reparent_to(window->get_render());
    arrow_node_.set_color(0,0.5,1,1);
    arrow_node_.set_pos(0, 0, -100);
}

void Arrow::set_pos(LPoint3f pos) {
    arrow_node_.set_pos(pos);
    //arrow_node_.set_z(1);
}

void Arrow::look_at(LPoint3f target, LPoint3f vector) {
    arrow_node_.look_at(target);
    LVector3f zVec = LVector3f(0, 0, 1);
    LVector3f dir = target - arrow_node_.get_pos();
    arrow_node_.set_sz(5*dir.length()/6.75);
    dir.normalize();
    arrow_node_.set_p(-dir.angle_deg(zVec));
    printf("Angle: %f", -dir.angle_deg(zVec));
}


}

}

