#include "../npc.h"

void npcState_sleep(int reason){

	switch(reason){
		case REASON_NONE:
			return;
			break;

		case REASON_LOS_TO_PLAYER:
			break;
	}
}
