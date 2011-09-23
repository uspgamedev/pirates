#include <stdio.h>
#include <string.h>
#include <math.h>
#include "filetype_code.h"


int main() {

	puts("Verify type sizes:");
	printf("uint8: %d\n", sizeof(uint8));
	printf("uint16: %d\n", sizeof(uint16));
	printf("uint32: %d\n", sizeof(uint32));
	printf("uint64: %d\n", sizeof(uint64));
	printf("char: %d\n", sizeof(char));
	printf("header: %d\n", sizeof(struct PWD_Header));
	printf("tile: %d\n", sizeof(struct PWD_Tile));

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
	strcpy(header.description, "3fort Description");
	header.size = 11;
	//memset(header.padding, 0, 1484);
	
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
	
	struct PWD_Tile tiles[num_tiles+2];
	for(a=0; a<num_tiles+2; a++){
		tiles[i].height = 8;
		tiles[i].texture = 1;
		tiles[i].player_start_location = 0; // 0 for none else player_id
		tiles[i].wind_module_min = 0;
		tiles[i].wind_module_max = 0;
		tiles[i].wind_angle_min = 0;
		tiles[i].wind_angle_max = 0;
		memset(tiles[i].padding, 0, 19);
	}

	tiles[5].player_start_location = 1;
	tiles[num_tiles-10].player_start_location = 2;

	tiles[49].height = 14;
	tiles[50].height = 15;
	tiles[51].height = 14;
	tiles[70].height = 12;
	tiles[71].height = 13;
	tiles[72].height = 12;
	tiles[100].height = 3;
	tiles[101].height = 2;
	tiles[102].height = 3;
	tiles[130].height = 6;
	tiles[131].height = 5;
	tiles[132].height = 6;

	for(i = 0; i < num_tiles + 2; i++)
		fwrite(&tiles[i], sizeof(struct PWD_Tile), 1, world);
		
	fclose(world);
	
	printf("\nGenerated file has size %u and %d tiles.\n", header.filesize, num_tiles);
	return 0;
}
