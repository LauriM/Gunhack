#include "../globals.h"
#include "../npc.h"

void npcState_idle(int id,int flags){
	//Hostile enemies attack always
	if(flags & SEE_PLAYER && flags & IS_HOSTILE){
		npcData[id].aiState = &npcState_attack;
		LOG_DEBUG_F("[ai] %i attacking!",id);
	}

	//Neutral enemies attack only when hurt
	if(flags & SEE_PLAYER && flags & IS_NEUTRAL && flags & IS_DAMAGED){

		npcData[id].aiState = &npcState_attack;
		LOG_DEBUG_F("[ai] %i attacking!",id);
	}

	//Peceful enemies flee
	if(flags & IS_DAMAGED && flags & SEE_PLAYER && flags & IS_DAMAGED){
		npcData[id].aiState = &npcState_flee;
		LOG_DEBUG_F("[ai] %i flee!",id);
	}
}
