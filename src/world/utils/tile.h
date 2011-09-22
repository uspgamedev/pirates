#ifndef PIRATES_WORLD_UTILS_TILE_H_
#define PIRATES_WORLD_UTILS_TILE_H_

namespace pirates {
namespace world {
namespace utils {

class Tile {
  public:
    Tile(int height);
	~Tile() {}

	int height() const { return height_; }

  private:
	int height_;

};

} // utils
} // world
} // pirates

#endif

