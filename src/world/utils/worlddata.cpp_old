#include "world/utils/worlddata.h"
#include "world/utils/tile.h"
#include <cstdio>
#include <cmath>

#define PRINT_ERROR_AND_RET_FALSE(file, filename, errormsg) { fclose(file); fprintf(stderr, errormsg, filename.c_str()); return false; }
#define PI 3.1415926535897932384626433832795f

#include "world/utils/filetype.h"

namespace pirates {

namespace world {

namespace utils {

WorldData::WorldData() : north_pole_(NULL), south_pole_(NULL) {
}

WorldData::~WorldData() {
    if(north_pole_) delete north_pole_;
    if(south_pole_) delete south_pole_;
    std::vector<Strip>::iterator it;
    for(it = strip_list_.begin(); it != strip_list_.end(); ++it) {
        Strip::iterator itstrip;
        for(itstrip = (*it).begin(); itstrip != (*it).end(); ++it) {
            delete (*itstrip);
        }
        (*it).clear();
    }
    strip_list_.clear();
}

static void CopyCharToWString(char* origin, std::wstring &target) {
	std::string text(origin);
	target = std::wstring(text.length(), L' ');
	std::copy(text.begin(), text.end(), target.begin());
}

const Tile* WorldData::tile_at(const WorldPoint& pos) {

    // Artificially create two more strips.
    int num_strips = strip_list_.size() + 2;


    // pos.theta_: Longitude. Says which strip the tile we want is at
    int stripid = num_strips * (1.0f - (pos.theta_ + PI/2) / PI);


    // The first and last "strips" only have one tile in then.
    if(stripid == 0)
        return north_pole_;
    else if(stripid == num_strips)
        return south_pole_;

    Strip srip = strip_list_[stripid - 1];

    // pos.phi_: Latitude. Says which tile of the strip the tile is.
    int tileid = (srip.size() - 1) * (pos.phi_ / (2.0f * PI));
    return srip[tileid];
}

int WorldData::CalculateIthStripSize(int strip_i) {
    return (int)(floor(2 * size_ * cos(strip_i * PI/size_)));
}

bool WorldData::Load(std::string filename) {
	// A single Tileset cannot be used to load more than one world file.
    if(strip_list_.size() > 0) return false;

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
	int strip_num = size_/2, inc = -1;
	for(register int ecx = 0; ecx < size_; ecx++) {
		num_tiles_ += CalculateIthStripSize(strip_num);
		if((strip_num += inc) == 0)
			inc = 1;
	}

    {   // North pole
		struct PWD_Tile tile;
		fread(&tile, sizeof(struct PWD_Tile), 1, file);
		north_pole_ = new Tile(tile.height);

        WorldPoint pos(PI/2.0f, 0.0f);
        north_pole_->set_position(pos);
    }

    {   // South pole
		struct PWD_Tile tile;
		fread(&tile, sizeof(struct PWD_Tile), 1, file);
		south_pole_ = new Tile(tile.height);

        WorldPoint pos(-PI/2.0f, 0.0f);
        south_pole_->set_position(pos);
    }

    fprintf(stderr, "Name: %s\n", header.world_name);

    // For each strip, read the tiles associated with it.
    strip_num = size_/2, inc = -1;
    for(register int ecx = 0; ecx < size_; ecx++) {
        // Calculate the number of tiles in this strip.
		int strip_size = CalculateIthStripSize(strip_num);

        float theta = ((strip_num * 1.0f) / (size_/2 + 1)) * (-inc) * PI/2.0f;
        Strip strip;
        for(int tile_i = 0; tile_i < strip_size; ++tile_i) {
            struct PWD_Tile pwtile;
		    fread(&pwtile, sizeof(struct PWD_Tile), 1, file);
            Tile *tile = new Tile(pwtile.height);

            float phi = ((tile_i * 1.0f) / strip_size) * 2.0f * PI;
            WorldPoint pos(theta, phi);
            tile->set_position(pos);
            strip.push_back(tile);
            fprintf(stderr, "S%d, T%d. Pos: %f %f\n", ecx, tile_i, theta, phi);
        }
        strip_list_.push_back(strip);

		if((strip_num += inc) == 0)
			inc = 1;
    }
	fclose(file);

    fprintf(stderr, "Stripcount: %d\n", strip_list_.size());


    {   WorldPoint pos(PI/2.0f, 0.0f);
        tile_at(pos); }
    {   WorldPoint pos(0.0f, 0.0f);
        tile_at(pos); }
    {   WorldPoint pos(-PI/2.0f, 0.0f);
        tile_at(pos); }

	return true;
}


} // namespace utils

} // namespace world

} // namespace pirates
