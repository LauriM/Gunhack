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

void mapCreateRoom(int z);

//TODO:Consider linked-list, then z would be changed to a pointer
void mapEditBox(int z,int boxX,int boxY,int width,int height,int tileType);

int mapSearchTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);

//Tile code to data
struct tile_s mapGetTileByPos(int z,int x,int y);

#endif
