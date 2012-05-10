#include "../globals.h"
#include "../npc.h"

void npcState_idle(int id,int flags){
	if(flags & SEE_PLAYER && flags & IS_HOSTILE){
		npcData[id].aiState = &npcState_attack;
		LOG_DEBUG_F("[ai] %i searching!",id);
	}
}
