#include "../npc.h"

void npcState_search(int id,int flags){
	if(flags & SEE_PLAYER){
		LOG_DEBUG("[ai] Attacking");
		npcData[id].aiState = &npcState_attack;
	}
}
