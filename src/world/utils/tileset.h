#ifndef PIRATES_WORLD_UTILS_TILESET_H_
#define PIRATES_WORLD_UTILS_TILESET_H_

#include <string>

namespace pirates {
namespace world {
namespace utils {

class Tile;

class Tileset {
  public:
    Tileset();
	~Tileset();
	bool Load(std::string& file);

  private:
	Tile** tile_list_;
	std::string loaded_file_;
	int num_tiles_;

};

} // utils
} // world
} // pirates

#endif

