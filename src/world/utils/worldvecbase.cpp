#include "worldvecbase.h"

#define PRECISION 1.0e-7

namespace pirates {
namespace world {
namespace utils {

// v = v1 + v2
WorldVector operator+(const WorldVector &left, const WorldVector &right) {
    return WorldVector(left.x_ + right.x_, left.y_ + right.y_, left.z_ + right.z_); 
}

// p = p1 + v2
WorldPoint operator+(const WorldPoint &left, const WorldVector &right) {
    // TODO: not exactly this
    return WorldPoint(left.x_ + right.x_, left.y_ + right.y_, left.z_ + right.z_); 
}

// p = v1 + p2
WorldPoint operator+(const WorldVector &left, const WorldPoint &right) {
    return right + left;
}

// v = -v1
WorldVector operator-(const WorldVector &left) {
    return WorldVector(-left.x_, -left.y_, -left.z_);
}

// v = p1 - p2
WorldVector operator-(const WorldPoint &left, const WorldPoint &right) {
    // TODO: not exactly this
    return WorldVector(left.x_ - right.x_, left.y_ - right.y_, left.z_ - right.z_); 
}

// p = p1 - v2
WorldPoint operator-(const WorldPoint &left, const WorldVector &right) {
    // TODO: not exactly this
    return WorldPoint(left.x_ - right.x_, left.y_ - right.y_, left.z_ - right.z_); 
}

// v = v1 - v2
WorldVector operator-(const WorldVector &left, const WorldVector &right) {
    return WorldVector(left.x_ - right.x_, left.y_ - right.y_, left.z_ - right.z_); 
}

// v = a * v2
WorldVector operator*(const float  &scalar, const WorldVector &right) {
    return right * scalar;
}

// v = v1 * a
WorldVector operator*(const WorldVector &left, const float   &scalar) {
    return WorldVector(left.x_ * scalar, left.y_ * scalar, left.z_ * scalar);
}

// v = v1 / a
WorldVector operator/(const WorldVector &left, const float   &scalar) {
    return left * (1.0f / scalar);
}

// a = v1 . v2
float operator*(const WorldVector &left, const WorldVector &right) {
    return left.x_ * right.x_ + left.y_ * right.y_ + left.z_ * right.z_;
}

// v1 == v2
bool operator==(const WorldVector &left, const WorldVector &right) {
    return (left.x_ - right.x_ < PRECISION) && (left.y_ - right.y_ < PRECISION) && (left.z_ - right.z_ < PRECISION);
}

// p1 == p2
bool operator==(const WorldPoint &left, const WorldPoint &right) {
    return (left.x_ - right.x_ < PRECISION) && (left.y_ - right.y_ < PRECISION) && (left.z_ - right.z_ < PRECISION);
}


const WorldVector WorldVector::ZERO = WorldVector();
const WorldVector WorldVector::UNIT_X = WorldVector(1.0f, 0.0f, 0.0f);
const WorldVector WorldVector::UNIT_Y = WorldVector(0.0f, 1.0f, 0.0f);
const WorldVector WorldVector::UNIT_Z = WorldVector(0.0f, 0.0f, 1.0f);

}
}
}