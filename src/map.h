#ifndef MAP_H
#define MAP_H

#include "render.h"

typedef enum{
	TILE_VOID = 0,
	TILE_EMPTY,
	TILE_ROCK,
	TILE_INDESTRUCTIBLE_ROCK,
	TILE_STAIRS_UP,
	TILE_STAIRS_DOWN,
	TILE_COUNT
} tiletype_t;

typedef enum{
	ROOM_TYPE_UNINITIALIZED = 0,
	ROOM_TYPE_DUNGEON
} roomtype_t;

typedef struct tile_s{
	char symbol;
	tiletype_t block;
} tile_t;

typedef struct room_s{
	int mapData[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
	color_t colorData[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
	int visData[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];//Contains the data that player has seen
	roomtype_t roomType;
} room_t;

extern room_t room[WORLD_ROOM_COUNT]; //Room list
extern tile_t tileInfo[TILE_COUNT];

void worldInit(void);
void roomInit(int id);
void tileInit(void);

void mapRender(void);
void mapCreateRoom(int z);
void mapScanFov(void);

void mapEditPoint(pos_t pos,tiletype_t tileType);
void mapEditBox(int id,int boxX,int boxY,int width,int height,int tileType);
void mapEditColorPoint(pos_t pos,color_t color);

int mapSearchTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);
int mapCheckTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);

pos_t mapFindTilePos(int roomId,tiletype_t tileType);

int mapLosCheck(int x,int y,int x2,int y2);
int mapLosCheckByPos(pos_t pos1,pos_t pos2);

struct tile_s* mapGetTileByPos(pos_t pos);
struct tile_s* mapGetVisByPos(pos_t pos);

void mapCheatSeeAll(void);

#endif
