#include <stdio.h>
#include <string.h>
#include <math.h>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;


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

int main() {
	FILE *world = fopen("3fort.pwd", "wb");
	struct PWD_Header header;
	strncpy(header.mime, "PWLD", 4);
	header.endian_check = 0x8910;
	header.version = 1;
	header.filesize = sizeof(struct PWD_Header);
	header.number_of_players = 2;
	header.ocean_height = 10;
	strcpy(header.world_name, "3fort");
	strcpy(header.thumbnail_path, "thumbnail.png");
	strcpy(header.gametype, "Melee");
	strcpy(header.description, "3forst Description");
	header.size = 11;
	memset(header.padding, 0, 1484);
	
	header.filesize += sizeof(struct PWD_Tile) * 2;
	
	int num_tiles = 0;
	
	int i = header.size/2, inc = -1;
	int a;
	for(a = 0; a < header.size; a++) {
		num_tiles += floor(2 * header.size * cos(i * 3.14159f/header.size));
		if((i += inc) == 0)
			inc = 1;
	}
	
	header.filesize += sizeof(struct PWD_Tile) * num_tiles;
	
	fwrite(&header, sizeof(struct PWD_Header), 1, world);
	
	struct PWD_Tile tile;
	tile.height = 8;
	tile.texture = 1;
	tile.player_start_location = 0; // 0 for none else player_id
	tile.wind_module_min = 0;
	tile.wind_module_max = 0;
	tile.wind_angle_min = 0;
	tile.wind_angle_max = 0;
	memset(tile.padding, 0, 19);
	
	for(i = 0; i < num_tiles + 2; i++)
		fwrite(&tile, sizeof(struct PWD_Tile), 1, world);
		
	fclose(world);
	return 0;
}