#ifndef FILETYPE_H_
#define FILETYPE_H_

#include "pstdint.h"

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
struct PWD_Header {
	// About the file
	/*    4 */char mime[4]; // Alyways PWLD
	/*	  2	*/uint16 endian_check; //0x8910
	/*    1 */uint8 version;
	/*    4 */uint32 filesize; // Remember to actually use this safeguard.
	
	// About the World
	/*    1 */uint8 number_of_players;
	/*    4 */uint32 ocean_height;
	
	/*  256 */char world_name[256]; // UTF-8 Encoded
	/*  256 */char thumbnail_path[256];
	/*   32 */char gametype[32]; // UTF-8 Encoded
	/* 1024 */char description[1024]; // UTF-8 Encoded (be careful when treating the input for this)
	/*    4 */uint32 size; // Always an odd number.
	
	char padding[1484]; // Header is exactly 3072 bytes (3 KiB) long. (Fill padding with 0x00)
};
struct PWD_Tile {
	/*    4 */uint32 height;
	/*    2 */uint16 texture;
	/*    1 */uint8 player_start_location; // 0 for none else player_id
	
	//Wind
	/*    1 */uint8 wind_module_min;
	/*    1 */uint8 wind_module_max;
	/*	  2 */uint16 wind_angle_min;
	/*	  2 */uint16 wind_angle_max;

	char padding[19]; // Tile is exactly 32 bytes long. (Fill padding with 0x00)
};

#endif
