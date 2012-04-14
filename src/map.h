#ifndef MAP_H
#define MAP_H

#include "globals.h"

//TODO: getcurrentroom, etc
//TODO: Fix rest of the extern bs

enum{
	TILE_VOID = 0,
	TILE_EMPTY,
	TILE_ROCK,
	TILE_COUNT,
	TILE_STAIRS_UP,
	TILE_STAIRS_DOWN
};

struct tile_s{
	char symbol;
	int visBlock;
	int moveBlock;
};

struct room_s{
	int mapData[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
	int colorData[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
	int visData[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];//Contains the data that player has seen
	int roomType;

	//TODO: If system to have rooms left/right int x/y/z should be added to the room. Atm the system can just use the currentRoom as depth
};

//---
extern int currentRoom;
extern int world[WORLD_SIZE_Z][WORLD_SIZE_X][WORLD_SIZE_Y]; //Contains RoomId
extern struct room_s room[WORLD_ROOM_COUNT]; //Room list
extern struct tile_s tileInfo[TILE_COUNT];
//---

void worldInit(void);
void roomInit(int id);
void tileInit(void);

void mapRender(void);
void mapCreateRoom(int z);
void mapScanFov(void);

void mapEditPoint(int id,int x,int y,int tileType);
void mapEditBox(int id,int boxX,int boxY,int width,int height,int tileType);

int mapSearchTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);
int mapCheckTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);

int mapLosCheck(int x,int y,int x2,int y2,int wallFix);

struct tile_s mapGetTileByPos(int z,int x,int y);
struct tile_s mapGetVisByPos(int id,int x,int y);

void mapCheatSeeAll(void);

#endif
