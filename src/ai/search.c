#include "../globals.h"
#include "../npc.h"

void npcState_search(int id,int flags){
	if(flags & SEARCH_DONE){
		npcData[id].aiState = &npcState_idle;
	}

	if(flags & SEE_PLAYER){
		LOG_DEBUG_F("[ai] %i Attacking",id);
		npcData[id].aiState = &npcState_attack;
	}

	// no need to update information for hearing the player, the position is changed when noise is detected.
}
