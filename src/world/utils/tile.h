#ifndef PIRATES_WORLD_UTILS_TILE_H_
#define PIRATES_WORLD_UTILS_TILE_H_

#include "world/utils/worldpos.h"

namespace pirates {
namespace world {
namespace utils {

class Tile {
  public:
    Tile(int height);
	~Tile() {}

    // Getters
    const WorldPos& position() const { return position_; }
	int height() const { return height_; }

    // Setters
    void set_position(const WorldPos& position) { position_ = position; }

  private:
	int height_;
    WorldPos position_;

};

} // utils
} // world
} // pirates

#endif

