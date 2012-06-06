#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "debug.h"
#include "math.h"

#define __GAME_NAME__ "Roguelike 0.8 preAlpha"
//Global macros
#define _TO_STRING(value) #value
#define TO_STRING(value) _TO_STRING(value)

//Debugging
//#define DEBUG_NO_LOG_DEBUG
#define DEBUG_LOG_SHOW_INGAME

#include "log.h"

#define DEBUG_BREAKPOINT __asm__("int $3");

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

//Game settings
#define PLAYER_MAX_CARRY 20

//Room generator settings
#define GEN_ROOM_MAX_COUNT 20
#define GEN_ROOM_MIN_COUNT 8
#define GEN_TUNNEL_COUNT_MIN 10
#define GEN_TUNNEL_COUNT_MAX 30
#define GEN_TUNNEL_MAX_LENGTH 40
#define GEN_TUNNEL_MAX_ROTATE_STEP 30
#define GEN_TUNNEL_MIN_ROTATE 10
#define GEN_TUNNEL_MAX_ROTATE 20

#define MAP_MAX_WIDTH  80
#define MAP_MAX_HEIGHT 20

#define GEN_MAX_TRIES 200

#define GEN_ITEM_COUNT_MIN 5
#define GEN_ITEM_COUNT_MAX 25

#define GEN_NPC_COUNT_MIN 5
#define GEN_NPC_COUNT_MAX 25

//Hud
#define STATUSLINE_MAX_CHARS 100

//Pathfind
#define PATHFIND_MAX_DEPTH 100

//Msg

#define MSG_HISTORY_MAX_COUNT 20
