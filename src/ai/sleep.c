#include "../npc.h"

void npcState_sleep(int id,int flags){
	if(flags & SEE_PLAYER){
		if(flags & IS_HOSTILE){
			npcData[id].aiState = &npcState_attack;
			LOG_DEBUG("[ai] Attacking");
		}else{
			npcData[id].aiState = &npcState_idle;
			LOG_INFO("[ai] idling!");
		}
	}
}
