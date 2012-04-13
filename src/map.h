#ifndef MAP_H
#define MAP_H

#include "globals.h"

//TODO: getcurrentroom, etc
//TODO: Fix rest of the extern bs

enum{
	TILE_EMPTY = 0,
	TILE_ROCK,
	TILE_COUNT
};

struct tile_s{
	char symbol;
	int visBlock;
	int moveBlock;
};

struct room_s{
	int mapData[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
	int roomType;
};

//---
extern int currentRoom;
extern int world[WORLD_SIZE_Z][WORLD_SIZE_X][WORLD_SIZE_Y]; //Contains RoomId
extern struct room_s room[WORLD_ROOM_COUNT]; //Room list
extern struct tile_s tileInfo[TILE_COUNT];
//---

void worldInit(void);
void roomInit(int id);
void mapRender(void);
void tileInit(void);

void mapCreateRoom(int z);

void mapEditPoint(int id,int x,int y,int tileType);
void mapEditBox(int id,int boxX,int boxY,int width,int height,int tileType);

int mapSearchTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);
int mapCheckTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);

//Tile code to data
struct tile_s mapGetTileByPos(int z,int x,int y);

#endif
