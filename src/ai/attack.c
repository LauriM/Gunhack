#include "../globals.h"
#include "../npc.h"

void npcState_attack(int id,int flags){
	if((flags & SEE_PLAYER) == 0){
		npcData[id].aiState = &npcState_search;
		LOG_DEBUG_INT("[ai] %i searching!",id);
	}

}
