#include "globals.h"
#include "npc.h"

npc_t npcInfo[NPC_COUNT];

#define CREATE_NPC(p_symbol,p_id,p_name,p_color) npcInfo[p_id].symbol = p_symbol; npcInfo[p_id].name = TO_STRING(p_name); npcInfo[p_id].color = p_color;

void npcInit(void){
	CREATE_NPC('D',NPC_DUMMY,"Dummy",TERM_COLOR_DEFAULT);
}
