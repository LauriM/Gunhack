#ifndef NPC_H
#define NPC_H
#include "globals.h"
#include "render.h"

typedef void (*npcaistate_t)(int id,int flags);

extern void npcState_sleep(int id,int flags);
extern void npcState_attack(int id,int flags);
extern void npcState_idle(int id,int flags);
extern void npcState_search(int id,int flags);

typedef enum{
	SEE_PLAYER      = 1 << 0,
	CANT_SEE_PLAYER = 1 << 1,
	IS_HOSTILE      = 1 << 2,
	IS_NEUTRAL      = 1 << 3,
	IS_PEACEFUL     = 1 << 4,
	LOW_HP          = 1 << 5,
	SEARCH_DONE     = 1 << 6,
} npc_flags_t;

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
	NPC_RELATION_PEACEFUL,
} npcrelation_t;


typedef struct npc_s{
	char symbol;
	char *name;
	color_t color;
	int maxHp;
	int meleeDmg;
	npcrelation_t relation;
} npc_t;

typedef struct npcdata_s{
	npcname_t name;
	npc_state_t state;
	pos_t pos;
	int hp;
	pos_t playerLastKnownPosition;
	void(*aiState)(int,int);
} npcdata_t;

extern size_t npcDataCapacity;
extern size_t npcDataSize;

extern npcdata_t *npcData;
extern npc_t     npcInfo[NPC_COUNT];

extern void npcInit(void);
extern void npcSpawnRandom(int z);
extern void npcSpawn(pos_t pos,npcname_t id);
extern void npcClearFromLevel(int z);
extern void npcMoveToPos(int id,pos_t pos);

extern bool npcExistsInPos(pos_t pos);

extern void npcRender();
extern void npcAiTick();

extern bool npcApplyDamagePos(pos_t pos,int damage);
extern void npcKillById(int id);
extern void npcDumpState();

#define NPC_UPDATE_LAST_KNOWN_POSITION npcData[i].playerLastKnownPosition.x = playerGetInfo()->pos.x; npcData[i].playerLastKnownPosition.y = playerGetInfo()->pos.y; npcData[i].playerLastKnownPosition.z = playerGetInfo()->pos.z;

#endif
