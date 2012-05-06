#include "../npc.h"

void npcState_attack(int id,int flags){
	if(flags & SEE_PLAYER){
		//continue attack
	}else{
		npcData[id].aiState = &npcState_search;
		LOG_DEBUG("[ai] Searching");
	}

}
