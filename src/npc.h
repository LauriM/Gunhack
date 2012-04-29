#ifndef NPC_H
#define NPC_H
#include "render.h"

typedef enum {
	NPCSTATE_DEAD = 0,
	NPCSTATE_ALIVE
} npc_state_t;

typedef enum {
	NPC_AI_STATE_SLEEP = 0,
	NPC_AI_STATE_IDLE,
	NPC_AI_STATE_ATTACK,
	NPC_AI_STATE_FLEE
} npc_ai_state_t;

typedef enum {
	NPC_DUMMY = 0,
	NPC_COUNT
} npcname_t;


typedef struct npc_s{
	char symbol;
	char *name;
	color_t color;
} npc_t;

typedef struct npcdata_s{
	npc_state_t state;
	npcname_t name;
	pos_t pos;
	npc_ai_state_t aiState;
} npcdata_t;

extern npcdata_t npcData[NPC_MAX_COUNT];
extern npc_t     npcInfo[NPC_COUNT];

extern void npcInit(void);
extern void npcSpawnRandom(int z);
extern void npcSpawn(pos_t pos,npcname_t id);

extern void npcRender();

#endif