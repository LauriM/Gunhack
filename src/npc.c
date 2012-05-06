#include "globals.h"
#include "npc.h"
#include "map.h"
#include "item.h"
#include <stdbool.h>
#include "player.h"
#include <stdio.h>

npcdata_t npcData[NPC_MAX_COUNT];
npc_t npcInfo[NPC_COUNT];

#define CREATE_NPC(p_symbol,p_id,p_name,p_color,p_maxhp,p_rel) npcInfo[p_id].symbol = p_symbol; npcInfo[p_id].name = TO_STRING(p_name); npcInfo[p_id].color = p_color; npcInfo[p_id].maxHp = p_maxhp; npcInfo[p_id].relation = p_rel;

void npcInit(void){
	//--    symbol , id                , name            , color              , maxhp , relationship
	CREATE_NPC('D' , NPC_DUMMY         , "Dummy"         , TERM_COLOR_DEFAULT , 10    , NPC_RELATION_NEUTRAL);
	CREATE_NPC('d' , NPC_DUMMY_HOSTILE , "Hostile Dummy" , TERM_COLOR_DEFAULT , 10    , NPC_RELATION_HOSTILE);

	//Init the npcdata array
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		npcData[i].state   = NPCSTATE_DEAD;
		npcData[i].aiState = &npcState_sleep;
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
				npcSpawn(pos,randomMax(NPC_COUNT));
				done = true;
			}
		}
		npcCount--;
	}
}

void npcSpawn(pos_t pos,npcname_t id){
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state != NPCSTATE_DEAD)
			continue;

		npcData[i].state   = NPCSTATE_ALIVE;
		npcData[i].pos     = pos;
		npcData[i].aiState= &npcState_sleep;
		npcData[i].name    = id;
		npcData[i].hp      = npcInfo[id].maxHp;

		LOG_INFO("NPC spawned");
		return;
	}

	LOG_ERROR("NPC_MAX_COUNT is too small!");
}

void npcRender(){
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		if(mapLosCheck(playerGetInfo()->pos.x,playerGetInfo()->pos.y,npcData[i].pos.x,npcData[i].pos.y) == false)
			continue;

		printIntxy(npcData[i].pos.x,npcData[i].pos.y,npcInfo[npcData[i].name].symbol);
	}
}

void npcClearFromLevel(int z){
	ASSERT_ROOM(z);

	for(int i = 0;i < NPC_MAX_COUNT;i++){

		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		if(npcData[i].pos.z != z)
			continue;

		npcData[i].state = NPCSTATE_DEAD;
	}
}

bool npcExistsInPos(pos_t pos){
	ASSERT_POS_T(pos);

	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		if(npcData[i].pos.x == pos.x && npcData[i].pos.y == pos.y)
			return true;
	}

	return false;
}

bool npcApplyDamagePos(pos_t pos,int damage){
	ASSERT_POS_T(pos);

	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		if(npcData[i].pos.x != pos.x || npcData[i].pos.y != pos.y)
			continue;

		npcData[i].hp = npcData[i].hp - damage;

		//blood
		pos_t bloodPos;
		bloodPos = npcData[i].pos;
		bloodPos.x = bloodPos.x + randomRange(-1,1);
		bloodPos.y = bloodPos.y + randomRange(-1,1);

		mapEditColorPoint(bloodPos,TERM_COLOR_RED);
		

		if(npcData[i].hp < 0){
			//DEATH
			npcKillById(i);
			return true;
		}

		break;
	}

	return false;
}

void npcKillById(int id){
	npcData[id].state = NPCSTATE_DEAD;

	itemSpawn(npcData[id].pos,ITEM_CORPSE);
	LOG_INFO("Enemy down!");
}

void npcAiTick(){
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		int flags;

		switch(npcInfo[npcData[i].name].relation){
			case NPC_RELATION_HOSTILE:
				flags = IS_HOSTILE;
				break;
			case NPC_RELATION_NEUTRAL:
				flags = IS_NEUTRAL;
				break;
			case NPC_RELATION_PEACEFUL:
				flags = IS_PEACEFUL;
				break;
		}

		if(npcData[i].pos.x == npcData[i].playerLastKnownPosition.x && npcData[i].pos.y == npcData[i].playerLastKnownPosition.y){
			flags = flags + SEARCH_DONE; 
			LOG_DEBUG_INT("[ai] %i Search done!",i);
		}

		if(mapLosCheckByPos(npcData[i].pos,playerGetInfo()->pos) == true){
			flags = flags + SEE_PLAYER;
		}

		//==========================================================//
			(*npcData[i].aiState)(i,flags);
		//==========================================================//          

		if(npcData[i].aiState == *npcState_attack){
			if(flags & SEE_PLAYER){
				NPC_UPDATE_LAST_KNOWN_POSITION;
			}

			pos_t temp = mapPathfindStep(npcData[i].pos,playerGetInfo()->pos);
			npcData[i].pos.x = temp.x;
			npcData[i].pos.y = temp.y;
		}

		if(npcData[i].aiState == *npcState_idle){
			if(flags & SEE_PLAYER){
				NPC_UPDATE_LAST_KNOWN_POSITION;
			}
		}

		if(npcData[i].aiState == *npcState_search){
			pos_t temp = mapPathfindStep(npcData[i].pos,npcData[i].playerLastKnownPosition); //TODO: Manage situation where its not possible to get to the player
			npcData[i].pos.x = temp.x;
			npcData[i].pos.y = temp.y;
		}
	}
}
