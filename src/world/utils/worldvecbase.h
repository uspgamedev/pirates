#ifndef PIRATES_WORLD_UTILS_WORLDVECBASE_H_
#define PIRATES_WORLD_UTILS_WORLDVECBASE_H_

#include "world/utils/worldpoint.h"
#include "world/utils/worldvector.h"

namespace pirates {
namespace world {
namespace utils {

// v = v1 + v2
WorldVector operator+(const WorldVector &left, const WorldVector &right);

// p = p1 + v2
WorldPoint operator+(const WorldPoint &left, const WorldVector &right);

// p = v1 + p2
WorldPoint operator+(const WorldVector &left, const WorldPoint &right);

// v = -v1
WorldVector operator-(const WorldVector &left);

// v = p1 - p2
WorldVector operator-(const WorldPoint &left, const WorldPoint &right);

// p = p1 - v2
WorldPoint operator-(const WorldPoint &left, const WorldVector &right);

// v = v1 - v2
WorldVector operator-(const WorldVector &left, const WorldVector &right);

// v = a * v2
WorldVector operator*(const float  &scalar, const WorldVector &right);

// v = v1 * a
WorldVector operator*(const WorldVector &left, const float   &scalar);

// v = v1 / a
WorldVector operator/(const WorldVector &left, const float   &scalar);

// a = v1 . v2
float operator*(const WorldVector &left, const WorldVector &right);

// v1 == v2
bool operator==(const WorldVector &left, const WorldVector &right);

// p1 == p2
bool operator==(const WorldPoint &left, const WorldPoint &right);


}
}
}

#endif

