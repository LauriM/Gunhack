#ifndef MAP_H
#define MAP_H

#include "globals.h"

struct tile_s{
	char symbol;
	int visBlock;
	int moveBlock;
};

struct tile_s tileInfo[TILE_COUNT];

int mapData[MAP_MAX_DEPTH][MAP_MAX_WIDTH][MAP_MAX_HEIGHT];

void mapInit(void);
void mapRender(void);

//Tile code to data
struct tile_s mapGetTileByPos(int z,int x,int y);

#endif
