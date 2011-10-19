
#include "base/game.h"
#include "planet.h"
#include "camera.h"

#define CAMERA_VEL    500.0f
#define CAMERA_HEIGHT 150.0f

namespace pirates {

using base::Game;

namespace world {

    void Camera::Initialize() {
        LPoint3f planet_center = GAME()->planet()->center();
        up_ = LVector3f(0.0f,1.0f,0.0f);
        look_at_ = planet_center - GAME()->camera().get_pos();
        right_ = look_at_.cross(up_);
        right_.normalize();

        GAME()->camera().look_at(planet_center,up_);
    }
    
    void Camera::MoveCamera(int cam, float dt) {
        LPoint3f planet_center = GAME()->planet()->center();

        LPoint3f new_pos = GAME()->camera().get_pos() - up_*dt*CAMERA_VEL;
        new_pos -= planet_center;
        new_pos.normalize();
        new_pos *= (GAME()->planet()->height_at(new_pos) + CAMERA_HEIGHT);

        GAME()->camera().set_pos(new_pos);

        look_at_ = new_pos - planet_center;
        up_ -= up_.project(look_at_);
        up_.normalize();
        GAME()->camera().look_at(planet_center,up_);
    }

}

}