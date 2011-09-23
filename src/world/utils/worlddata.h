#ifndef PIRATES_WORLD_UTILS_WORLDDATA_H_
#define PIRATES_WORLD_UTILS_WORLDDATA_H_

#include <string>

namespace pirates {
namespace world {
namespace utils {

class Tile;

class WorldData {
  public:
    WorldData();
	~WorldData();
	bool Load(std::string file);

	//const unsigned long& size() const { return size_; }
	//const unsigned long& ocean_height() const { return ocean_height_; }
	//const std::wstring& name const { return name_; }
	//const std::wstring& description const { return description_; }

  private:
	Tile** tile_list_;
	std::string loaded_file_;
	int num_tiles_;

	unsigned long size_, ocean_height_;
	std::wstring name_, description_;

};

} // utils
} // world
} // pirates

#endif

