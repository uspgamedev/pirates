#ifndef PIRATES_WORLD_UTILS_WORLDDATA_H_
#define PIRATES_WORLD_UTILS_WORLDDATA_H_

#include <string>
#include <vector>
#include "world/utils/worldpoint.h"

namespace pirates {
namespace world {
namespace utils {

class Tile;

class WorldData {
  public:
    WorldData();
	~WorldData();
	bool Load(std::string file);

    typedef std::vector<Tile*> Strip;

    const Tile* tile_at(const WorldPoint& pos);

	const unsigned long& size() const { return size_; }
	const unsigned long& ocean_height() const { return ocean_height_; }
	//const std::wstring& name const { return name_; }
	//const std::wstring& description const { return description_; }

  private:
	Tile *north_pole_, *south_pole_;
    std::vector<Strip> strip_list_;

	std::string loaded_file_;
	int num_tiles_;

	unsigned long size_, ocean_height_;
	std::wstring name_, description_;

    int CalculateIthStripSize(int strip_i);

};

} // utils
} // world
} // pirates

#endif

