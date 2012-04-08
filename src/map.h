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
int mapContent[MAP_MAX_DEPTH][MAP_MAX_WIDTH][MAP_MAX_HEIGHT]; //TO BE DEPRACRATED

void mapInit(void);
void mapRender(void);

int mapGetContentChar(int code);

#endif
