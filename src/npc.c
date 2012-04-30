#include "globals.h"
#include "npc.h"
#include "map.h"
#include <stdbool.h>
#include "player.h"

npcdata_t npcData[NPC_MAX_COUNT];
npc_t npcInfo[NPC_COUNT];

#define CREATE_NPC(p_symbol,p_id,p_name,p_color,p_maxhp) npcInfo[p_id].symbol = p_symbol; npcInfo[p_id].name = TO_STRING(p_name); npcInfo[p_id].color = p_color; npcInfo[p_id].maxHp = p_maxhp;

void npcInit(void){
	//--    symbol   id         name       color                maxhp
	CREATE_NPC('D' , NPC_DUMMY , "Dummy" , TERM_COLOR_DEFAULT , 10);

	//Init the npcdata array

	for(int i = 0;i < NPC_MAX_COUNT;i++){
		npcData[i].state = NPCSTATE_DEAD;
	}
}

void npcSpawnRandom(int z){
	pos_t pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = z;

	int npcCount = randomRange(GEN_NPC_COUNT_MIN,GEN_NPC_COUNT_MAX);

	while(npcCount > 0){
		int done = false;
		while(done == false){
            //TODO: Check for existing enemies

			pos.x = randomRange(1,MAP_MAX_WIDTH-1);
			pos.y = randomRange(1,MAP_MAX_HEIGHT-1);

			if(mapGetTileByPos(pos)->block == 0){
				//TODO: Implement random npc type
				npcSpawn(pos,NPC_DUMMY);
				done = true;
			}
		}
		npcCount--;
	}
}

void npcSpawn(pos_t pos,npcname_t id){
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state == NPCSTATE_DEAD){
			npcData[i].state   = NPCSTATE_ALIVE;
			npcData[i].pos     = pos;
			npcData[i].aiState = NPC_AI_STATE_IDLE;
			npcData[i].name    = id;
			npcData[i].hp      = npcInfo[id].maxHp;

			LOG_INFO("NPC spawned");
			return;
		}
	}

	LOG_ERROR("NPC_MAX_COUNT is too small!");
}

void npcRender(){
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state == NPCSTATE_ALIVE){
			if(mapLosCheck(playerGetInfo()->pos.x,playerGetInfo()->pos.y,npcData[i].pos.x,npcData[i].pos.y) == true){
				printIntxy(npcData[i].pos.x,npcData[i].pos.y,npcInfo[npcData[i].name].symbol);
			}
		}
	}
}

void npcClearFromLevel(int z){
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state == NPCSTATE_ALIVE){
			if(npcData[i].pos.z == z){
				npcData[i].state = NPCSTATE_DEAD;
			}
		}
	}
}

bool npcExistsInPos(pos_t pos){
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state == NPCSTATE_ALIVE){
			if(npcData[i].pos.x == pos.x && npcData[i].pos.y == pos.y){
				return true;
			}
		}
	}

	return false;
}
