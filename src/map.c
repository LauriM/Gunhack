#include "map.h"
#include "render.h"
#include "globals.h"
#include "player.h"
#include "math.h"

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
	tileInfo[1].visBlock  = 1;
	tileInfo[1].moveBlock = 1;
}

void mapRender(void){

	int x,y;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			printIntxy(x,y,mapGetTileByPos(playerZ,x,y).symbol);
		}
	}
}

void mapCreateRoom(int z){
	int x,y;

	//First, lets add the concrete
	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			mapData[z][x][y] = 1;
		}
	}

	int roomX      = 0;
	int roomY      = 0;
	int roomWidth  = 0;
	int roomHeight = 0;

	roomX = playerX - 1;
	roomY = playerY - 1; 

	mapEditBox(z,roomX,roomY,3,3,0);

	int roomCount = (random(5)+2);

	for(int i = 0;i < roomCount;i++){
		roomWidth = (random(8) + 3);//TODO: change 5 to max size
		roomHeight = (random(8) + 3);
		roomX = random(MAP_MAX_WIDTH-roomWidth-1);
		roomY = random(MAP_MAX_HEIGHT-roomHeight-1);

		mapEditBox(z,roomX,roomY,roomWidth,roomHeight,0);
	}
}

void mapEditBox(int z,int boxX,int boxY,int width,int height,int tileType){
	for(int y = boxY;y < boxY + height;y++){
		for(int x = boxX;x < boxX + width;x++){
			mapData[z][x][y] = tileType;
		}
	}
}

struct tile_s mapGetTileByPos(int z,int x,int y){
	return tileInfo[mapData[z][x][y]];
}
