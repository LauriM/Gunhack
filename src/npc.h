#ifndef NPC_H
#define NPC_H
#include "globals.h"
#include "render.h"

typedef void (*npcaistate_t)(int id,int flags);

extern void npcState_sleep(int id,int flags);
extern void npcState_attack(int id,int flags);
extern void npcState_idle(int id,int flags);
extern void npcState_search(int id,int flags);
extern void npcState_flee(int id,int flags);

typedef enum{
	SEE_PLAYER      = 1 << 0,
	CANT_SEE_PLAYER = 1 << 1,
	IS_HOSTILE      = 1 << 2,
	IS_NEUTRAL      = 1 << 3,
	IS_PEACEFUL     = 1 << 4,
	IS_DAMAGED      = 1 << 5,
	SEARCH_DONE     = 1 << 6,
} npc_flags_t;

typedef enum {
	NPCSTATE_DEAD = 0,
	NPCSTATE_ALIVE
} npc_state_t;

typedef enum { /* TODO: Depracrated ? */
	NPC_AI_STATE_SLEEP = 0,
	NPC_AI_STATE_IDLE,
	NPC_AI_STATE_ATTACK,
	NPC_AI_STATE_FLEE,
	NPC_AI_STATE_SEARCH
} npc_ai_state_t;

typedef enum {
	NPC_GRID_BUG = 0,
	NPC_GNOME,
	NPC_GNOME_LORD,
	NPC_GNOME_SHAMAN,
	NPC_KOBOLD,
	NPC_KOBOLD_LORD,
	NPC_KOBOLD_SHAMAN,
	NPC_WOLF,
	NPC_DWARF,
	NPC_BUGBEAR,
	NPC_SPIDER,
	NPC_IMP,
	NPC_ORC,
	NPC_ORC_LORD,
	NPC_OGRE,
	NPC_OGRE_LORD,
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
	int meleeDmgMin;
	int meleeDmgMax;
	int level;//Level when can be spawned
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

size_t npcDataCapacity;
size_t npcDataSize;

npcdata_t *npcData;
npc_t     npcInfo[NPC_COUNT];

void npcInit(void);
void npcSpawnRandom(int z);
void npcSpawn(pos_t pos,npcname_t id);
npcname_t npcGiveRandomNpcByLevel(int level);
void npcClearFromLevel(int z);
void npcMoveToPos(int id,pos_t pos,bool allowAttack);

bool npcExistsInPos(pos_t pos);
int npcGetCharOnPos(pos_t pos);

void npcRender();
void npcAiTick();

bool npcApplyDamagePos(pos_t pos,int damage);
void npcKillById(int id);
void npcDumpState();

#define NPC_UPDATE_LAST_KNOWN_POSITION npcData[i].playerLastKnownPosition.x = playerGetInfo()->pos.x; npcData[i].playerLastKnownPosition.y = playerGetInfo()->pos.y; npcData[i].playerLastKnownPosition.z = playerGetInfo()->pos.z;

#endif
