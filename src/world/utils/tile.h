#ifndef PIRATES_WORLD_UTILS_TILE_H_
#define PIRATES_WORLD_UTILS_TILE_H_

#include "world/utils/worldpoint.h"

namespace pirates {
namespace world {
namespace utils {

class Tile {
  public:
    Tile(int height);
	~Tile() {}

    // Getters
    const WorldPoint& position() const { return position_; }
	int height() const { return height_; }

    // Setters
    void set_position(const WorldPoint& position) { position_ = position; }

  private:
	int height_;
    WorldPoint position_;

};

} // utils
} // world
} // pirates

#endif

