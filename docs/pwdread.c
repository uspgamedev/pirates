#include <stdio.h>
#include <string.h>
#include <math.h>
#include "filetype_code.h"

int main() {
	FILE *world = fopen("3fort.pwd", "rb");
	struct PWD_Header header;

	fread(&header, sizeof(struct PWD_Header), 1, world);

	int num_tiles = 0;
	
	int i = header.size/2, inc = -1;
	int a;
	for(a = 0; a < header.size; a++) {
		num_tiles += floor(2 * header.size * cos(i * 3.14159f/header.size));
		if((i += inc) == 0)
			inc = 1;
	}
	
	struct PWD_Tile tiles[num_tiles+2];

	for(i = 0; i < num_tiles + 2; i++)
		fread(&tiles[i], sizeof(struct PWD_Tile), 1, world);
		
	fclose(world);

	printf("Mime: '%c%c%c%c'\n", header.mime[0], header.mime[1], header.mime[2], header.mime[3]);
	printf("Endian Check: 0x%x\n", header.endian_check);
	printf("Version: %u\n", header.version);
	printf("Filesize: %u\n", header.filesize);
	puts("");
	// About the World
	printf("Num Players: %u\n", header.number_of_players);
	printf("Ocean Height: %u\n", header.ocean_height);
	puts("");
	printf("World Name: '%s'\n", header.world_name);
	printf("Thumbnail: '%s'\n", header.thumbnail_path);
	printf("Gametype: '%s'\n", header.gametype);
	printf("Description: '%s'\n", header.description);
	printf("Size: %u\n", header.size);
	return 0;
}
