#include "world/utils/tileset.h"
#include <cstdio>

namespace pirates {

namespace world {

namespace utils {

Tileset::Tileset() : tile_list_(NULL) {
}

Tileset::~Tileset() {
	if(tile_list_ != NULL) {
		for(int i = 0; i < num_tiles_; ++i)
			delete tile_list_[i];
		delete tile_list_;
	}
}

bool Tileset::Load(std::string& filename) {
	// A single Tileset cannot be used to load more than one world file.
	if(tile_list_ != NULL) return false;

	FILE* file = fopen(filename.c_str(), "rb");
	if(file == NULL) return false;

	loaded_file_ = filename;
	num_tiles_ = 5;
	tile_list_ = new Tile*[num_tiles_];
	for(int i = 0; i < num_tiles_; ++i)
		tile_list_[i] = NULL;

	fclose(file);
	return true;
}


} // namespace utils

} // namespace world

} // namespace pirates
