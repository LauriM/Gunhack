#ifndef NPC_H
#define NPC_H
#include "globals.h"
#include "render.h"

typedef enum {
	NPCSTATE_DEAD = 0,
	NPCSTATE_ALIVE
} npc_state_t;

typedef enum {
	NPC_AI_STATE_SLEEP = 0,
	NPC_AI_STATE_IDLE,
	NPC_AI_STATE_ATTACK,
	NPC_AI_STATE_FLEE,
	NPC_AI_STATE_SEARCH
} npc_ai_state_t;

typedef enum {
	NPC_DUMMY = 0,
	NPC_DUMMY_HOSTILE,
	NPC_COUNT
} npcname_t;

typedef enum {
	NPC_RELATION_NEUTRAL = 0,
	NPC_RELATION_HOSTILE,
	NPC_RELATION_PEACFUL,
} npcrelation_t;


typedef struct npc_s{
	char symbol;
	char *name;
	color_t color;
	int maxHp;
	npcrelation_t relation;
} npc_t;

typedef struct npcdata_s{
	npcname_t name;
	npc_state_t state;
	npc_ai_state_t aiState;
	pos_t pos;
	int hp;
	pos_t playerLastKnownPosition;
} npcdata_t;

extern npcdata_t npcData[NPC_MAX_COUNT];
extern npc_t     npcInfo[NPC_COUNT];

extern void npcInit(void);
extern void npcSpawnRandom(int z);
extern void npcSpawn(pos_t pos,npcname_t id);
extern void npcClearFromLevel(int z);

extern bool npcExistsInPos(pos_t pos);

extern void npcRender();
extern void npcAiTick();

extern bool npcApplyDamagePos(pos_t pos,int damage);
extern void npcKillById(int id);

#endif
