#include "world/utils/worlddata.h"
#include "world/utils/tile.h"
#include <cstdio>
#include <cmath>

#define PRINT_ERROR_AND_RET_FALSE(file, filename, errormsg) { fclose(file); fprintf(stderr, errormsg, filename.c_str()); return false; }
#define PI 3.1415926535897932384626433832795

#include "world/utils/filetype.h"

namespace pirates {

namespace world {

namespace utils {

WorldData::WorldData() : tile_list_(NULL) {
}

WorldData::~WorldData() {
	if(tile_list_ != NULL) {
		for(int i = 0; i < num_tiles_; ++i)
			delete tile_list_[i];
		delete tile_list_;
	}
}

static void CopyCharToWString(char* origin, std::wstring &target) {
	std::string text(origin);
	target = std::wstring(text.length(), L' ');
	std::copy(text.begin(), text.end(), target.begin());
}

const Tile* WorldData::tile_at(const WorldPos& pos) {
    return NULL;
}

bool WorldData::Load(std::string filename) {
	// A single Tileset cannot be used to load more than one world file.
	if(tile_list_ != NULL) return false;

	FILE* file = fopen(filename.c_str(), "rb");
	if(file == NULL) return false;


	struct PWD_Header header;
	fread(&header, sizeof(struct PWD_Header), 1, file);
	if(strncmp(header.mime, "PWLD", 4) != 0 || (header.endian_check != 0x8910 && header.endian_check != 0x1089)) 
		PRINT_ERROR_AND_RET_FALSE(file, filename, "Error in WorldData::Load. File '%s' is not a valid pwd file.\n");

	if(header.endian_check != 0x8910)
		PRINT_ERROR_AND_RET_FALSE(file, filename, "Error in WorldData::Load. File '%s' has incorrect endianness.\n");

	// Setting filename
	loaded_file_ = filename;

	// Storing values for getters
	size_ = header.size;
	CopyCharToWString(header.world_name, name_);
	CopyCharToWString(header.description, description_);
	ocean_height_ = header.ocean_height;


	// Calculate the number of tiles
	num_tiles_ = 0;	
	int i = size_/2, inc = -1;
	int a;
	for(a = 0; a < size_; a++) {
		num_tiles_ += (int)(floor(2 * size_ * cos(i * PI/size_)));
		if((i += inc) == 0)
			inc = 1;
	}

	tile_list_ = new Tile*[num_tiles_];

    {   // North pole
		struct PWD_Tile tile;
		fread(&tile, sizeof(struct PWD_Tile), 1, file);
		tile_list_[0] = new Tile(tile.height);

        WorldPos pos(PI/2.0f, 0.0f);
        tile_list_[0]->set_position(pos);
    }

    {   // South pole
		struct PWD_Tile tile;
		fread(&tile, sizeof(struct PWD_Tile), 1, file);
		tile_list_[1] = new Tile(tile.height);

        WorldPos pos(-PI/2, 0.0f);
        tile_list_[1]->set_position(pos);
    }

    // ???? Preciso lembrar pq tem isso
    for(int strip_num = 0; strip_num < size_; ++strip_num) {
    }


    // READ THE TILES!
	for(int i = 2; i < num_tiles_; ++i) {
		struct PWD_Tile tile;
		fread(&tile, sizeof(struct PWD_Tile), 1, file);
		tile_list_[i] = new Tile(tile.height);
	}

	fclose(file);
	printf("Name: %s\n", header.world_name);
	return true;
}


} // namespace utils

} // namespace world

} // namespace pirates
