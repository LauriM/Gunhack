#include <assert.h>
#include <stdbool.h>
#include "debug.h"
#include "math.h"

//Global macros
#define _TO_STRING(value) #value
#define TO_STRING(value) _TO_STRING(value)

//Debugging
#define DEBUG_NO_LOG_DEBUG
#include "log.h"

#ifndef GLOBALS_H
#define GLOBALS_H
typedef enum{
	DIR_N = 0,
	DIR_S,
	DIR_E,
	DIR_W,
	DIR_NE,
	DIR_SE,
	DIR_SW,
	DIR_NW,
	DIR_COUNT
} dir_t;
#endif

//Room generator settings
#define GEN_ROOM_MAX_COUNT 20
#define GEN_ROOM_MIN_COUNT 8
#define GEN_TUNNEL_COUNT_MIN 10
#define GEN_TUNNEL_COUNT_MAX 30
#define GEN_TUNNEL_MAX_LENGTH 40
#define GEN_TUNNEL_MAX_ROTATE_STEP 30
#define GEN_TUNNEL_MIN_ROTATE 10
#define GEN_TUNNEL_MAX_ROTATE 20

#define WORLD_ROOM_COUNT 50

#define MAP_MAX_WIDTH  80
#define MAP_MAX_HEIGHT 21

#define GEN_MAX_TRIES 200

#define GEN_ITEM_COUNT_MIN 5
#define GEN_ITEM_COUNT_MAX 25

#define GEN_NPC_COUNT_MIN 5
#define GEN_NPC_COUNT_MAX 25

//Hud
#define STATUSLINE_MAX_CHARS 100

//Items
#define ITEM_MAX_COUNT 100 //How many items can be in the game

//Npc

#define NPC_MAX_COUNT 100
