#include "../globals.h"
#include "../npc.h"

void npcState_search(int id,int flags){
	if(flags & SEARCH_DONE){
		npcData[id].aiState = &npcState_idle;
	}

	if(flags & SEE_PLAYER){
		LOG_DEBUG_INT("[ai] %i Attacking",id);
		npcData[id].aiState = &npcState_attack;
	}
}
