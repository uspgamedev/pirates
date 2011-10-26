#ifndef PIRATES_WORLD_UTILS_WORLDVECTOR_H_
#define PIRATES_WORLD_UTILS_WORLDVECTOR_H_

#include "lvector2.h"
#include <cmath>

namespace pirates {
namespace world {
namespace utils {

class WorldVector {
  public:
    WorldVector() : x_(0.0f), y_(0.0f), z_(0.0f) {}
    WorldVector(float fill) : x_(fill), y_(fill), z_(fill) {}
    WorldVector(float x, float y, float z) : x_(x), y_(y), z_(z) {}

	union {
		struct { float x_, y_, z_; };
		struct { float theta_, phi_, r_; };
		struct { float val_[3]; };
	};



    
    LVector3f ConvertToPandaVector() const { return LVector3f(x_, y_, z_); }

    WorldVector CrossProduct(WorldVector &v) const {
        // http://en.wikipedia.org/wiki/Cross_product
        return WorldVector(y_ * v.z_ - z_ * v.y_, z_ * v.x_ - x_ * v.z_, x_ * v.y_ - y_ * v.x_);
    }

    float LengthSquared() const { return x_*x_ + y_*y_ + z_*z_; }
    float Length() const { return sqrt(LengthSquared()); }

    void Normalize() {  float length = Length(); for(int i = 0; i < 3; ++i ) val_[i] /= length; }
    WorldVector Normalized() const { 
        float length = Length(); 
        return WorldVector(x_ / length, y_ / length, z_ / length);
    }


    WorldVector operator+= (const WorldVector& right) {
        x_ += right.x_;
        y_ += right.y_;
        z_ += right.z_;
        return *this;
    }

    WorldVector operator*= (const float &scalar) {
        x_ *= scalar;
        y_ *= scalar;
        z_ *= scalar;
        return *this;
    }

    const static WorldVector ZERO, UNIT_X, UNIT_Y, UNIT_Z;
};

}
}
}

#endif

