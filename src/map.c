#include "map.h"
#include "render.h"
#include "globals.h"

extern struct tile_s tileInfo[TILE_COUNT];

void mapInit(void){
	int z,x,y;

	for(z = 0;z < MAP_MAX_DEPTH;z++){
		for(x = 0;x < MAP_MAX_WIDTH;x++){
			for(y = 0;y < MAP_MAX_HEIGHT;y++){
				mapContent[z][x][y] = 0;
			}
		}
	}

	tileInfo[0].symbol    = ' ';
	tileInfo[0].visBlock  = 0;
	tileInfo[0].moveBlock = 0;

	tileInfo[1].symbol    = '#';
	tileInfo[1].visBlock  = 0;
	tileInfo[1].moveBlock = 0;
}

void mapRender(void){

	int z,x,y;

	for(z = 0;z < MAP_MAX_DEPTH;z++){
		for(x = 0;x < MAP_MAX_WIDTH;x++){
			for(y = 0;y < MAP_MAX_HEIGHT;y++){
				printIntxy(x,y,mapGetContentChar(mapContent[z][x][y]));
			}
		}
	}
}

int mapGetContentChar(int code){
	return tileInfo[code].symbol;
}
