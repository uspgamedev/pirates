#ifndef PIRATES_WORLD_UTILS_WORLDPOINT_H_
#define PIRATES_WORLD_UTILS_WORLDPOINT_H_

#include "lPoint3.h"
#include <cmath>

namespace pirates {
namespace world {
namespace utils {

// Theta is the longitude and belongs to the interval [-PI/2,PI/2].
// Phi is the latitude and belongs to the interval [0, 2PI].
// (PI/2, 0) is the north pole, while (-PI/2, 0) is the south pole.

class WorldPoint {
  public:
    WorldPoint() : theta_(0.0f), phi_(0.0f), r_(-1.0f) {}
    WorldPoint(float theta, float phi) : theta_(theta), phi_(phi), r_(-1.0f) {}
    WorldPoint(float theta, float phi, float r) : theta_(theta), phi_(phi), r_(r) {}
    
    LPoint3f ConvertToPandaPoint() const {
        assert(r_ > 0.0f);
        float sinTheta = sin(theta_);
        return LPoint3f(r_ * sinTheta * cos(phi_), r_ * sinTheta * sin(phi_), r_ * cos(theta_));
    }

    // getters
	union {
		struct { float x_, y_, z_; };
		struct { float theta_, phi_, r_; };
		struct { float val_[3]; };
	};
};

}
}
}

#endif

