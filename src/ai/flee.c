#include "../globals.h"
#include "../npc.h"

void npcState_flee(int id,int flags){
	if((flags & SEE_PLAYER) == 0){
		npcData[id].aiState = &npcState_idle;
		LOG_DEBUG_F("[ai] %i idling!",id);
	}
}
