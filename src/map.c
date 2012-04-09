#include "map.h"
#include "render.h"
#include "globals.h"
#include "player.h"

extern struct tile_s tileInfo[TILE_COUNT];

extern int playerZ;

void mapInit(void){
	int z,x,y;

	for(z = 0;z < MAP_MAX_DEPTH;z++){
		for(x = 0;x < MAP_MAX_WIDTH;x++){
			for(y = 0;y < MAP_MAX_HEIGHT;y++){
				mapData[z][x][y] = 0;
			}
		}
	}

	tileInfo[0].symbol    = ' ';
	tileInfo[0].visBlock  = 0;
	tileInfo[0].moveBlock = 0;

	tileInfo[1].symbol    = '#';
	tileInfo[1].visBlock  = 0;
	tileInfo[1].moveBlock = 0;

	mapData[0][10][10] = 1;
	mapData[0][11][10] = 1;
	mapData[0][10][11] = 1;
	mapData[0][10][12] = 1;
}

void mapRender(void){

	int x,y;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			printIntxy(x,y,mapGetTileByPos(playerZ,x,y).symbol);
		}
	}
}

struct tile_s mapGetTileByPos(int z,int x,int y){
	return tileInfo[mapData[z][x][y]];
}
