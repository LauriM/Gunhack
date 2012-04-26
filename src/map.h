#ifndef MAP_H
#define MAP_H

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
	int colorData[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
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

void mapEditPoint(int id,int x,int y,int tileType);
void mapEditBox(int id,int boxX,int boxY,int width,int height,int tileType);

int mapSearchTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);
int mapCheckTileCoords(int z,int boxX,int boxY,int width,int height,int tileType);

pos_t mapFindTilePos(int roomId,tiletype_t tileType);

int mapLosCheck(int x,int y,int x2,int y2);

struct tile_s* mapGetTileByPos(int z,int x,int y);
struct tile_s* mapGetVisByPos(int z,int x,int y);

void mapCheatSeeAll(void);

#endif
