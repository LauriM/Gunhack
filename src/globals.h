#include <assert.h>

//directions
#define DIR_N 0
#define DIR_S 1
#define DIR_E 2
#define DIR_W 3

//Room generator settings
#define GEN_ROOM_MAX_COUNT 5
#define GEN_ROOM_MIN_COUNT 5

#define WORLD_SIZE_Z 10
#define WORLD_SIZE_X 50
#define WORLD_SIZE_Y 50
#define WORLD_ROOM_COUNT (WORLD_SIZE_Z * WORLD_SIZE_X * WORLD_SIZE_Y)

#define MAP_MAX_WIDTH  80
#define MAP_MAX_HEIGHT 21

#define TILE_COUNT 2

#define GEN_MAX_TRIES 200
