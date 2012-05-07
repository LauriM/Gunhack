#include "globals.h"
#include "npc.h"
#include "map.h"
#include "item.h"
#include <stdbool.h>
#include "player.h"
#include <math.h>

npcdata_t npcData[NPC_MAX_COUNT];
npc_t npcInfo[NPC_COUNT];

#define CREATE_NPC(p_symbol,p_id,p_name,p_color,p_maxhp,p_rel,p_meleedmg) npcInfo[p_id].symbol = p_symbol; npcInfo[p_id].name = TO_STRING(p_name); npcInfo[p_id].color = p_color; npcInfo[p_id].maxHp = p_maxhp; npcInfo[p_id].relation = p_rel; npcInfo[p_id].meleeDmg = p_meleedmg;

void npcInit(void){
	//--    symbol , id                , name            , color              , maxhp , relationship         , meleeDMG
	CREATE_NPC('D' , NPC_DUMMY         , "Dummy"         , TERM_COLOR_DEFAULT , 10    , NPC_RELATION_NEUTRAL , 1);
	CREATE_NPC('d' , NPC_DUMMY_HOSTILE , "Hostile Dummy" , TERM_COLOR_DEFAULT , 10    , NPC_RELATION_HOSTILE , 1);

	//Init the npcdata array
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		npcData[i].state   = NPCSTATE_DEAD;
		npcData[i].aiState = &npcState_sleep;
	}
}

void npcSpawnRandom(int z){
	ASSERT_ROOM(z);
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
				npcSpawn(pos,randomMax(NPC_COUNT-1));
				done = true;
			}
		}
		npcCount--;
	}
}

void npcSpawn(pos_t pos,npcname_t id){
	assert(id < NPC_COUNT);
	assert(id >= 0);

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

		if(mapLosCheckByPos(playerGetInfo()->pos,npcData[i].pos) == true){
			flags = flags + SEE_PLAYER;
		}

		//==========================================================//
			(*npcData[i].aiState)(i,flags);
		//==========================================================//          

		if(npcData[i].aiState == *npcState_attack){
			if(flags & SEE_PLAYER){
				NPC_UPDATE_LAST_KNOWN_POSITION;
			}

			pos_t tempPos = mapPathfindStep(npcData[i].pos,playerGetInfo()->pos);
			npcMoveToPos(i,tempPos);
		}

		if(npcData[i].aiState == *npcState_idle){
			if(flags & SEE_PLAYER){
				NPC_UPDATE_LAST_KNOWN_POSITION;
			}

			//move randomly
			pos_t randPos;
			randPos.z = npcData[i].pos.z;
			randPos.x = (npcData[i].pos.x + randomRange(-1,1));
			randPos.y = (npcData[i].pos.y + randomRange(-1,1));

			npcMoveToPos(i,randPos);
		}

		if(npcData[i].aiState == *npcState_search){
			pos_t tempPos = mapPathfindStep(npcData[i].pos,npcData[i].playerLastKnownPosition); //TODO: Manage situation where its not possible to get to the player
			npcMoveToPos(i,tempPos);
		}
	}
}

void npcMoveToPos(int id,pos_t pos){
    //Check that the distance isn't longer than one tile.

	if(fabs(npcData[id].pos.x - pos.x) > 1 || fabs(npcData[id].pos.y - pos.y) > 1){
		LOG_ERROR("Npc tries to jump tiles!");
		return;
	}

	if(playerGetInfo()->pos.x == pos.x && playerGetInfo()->pos.y == pos.y){
		playerGetInfo()->hp = playerGetInfo()->hp - npcInfo[npcData[id].name].meleeDmg;
		return;
	}

	if(mapGetTileByPos(pos)->block == 1)
		return;

	if(npcExistsInPos(pos) == true)
		return;

	npcData[id].pos = pos;
}

void npcDumpState(){
	LOG_DEBUG("=======================");
	for(int i = 0;i < NPC_MAX_COUNT;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		char output[100];

		snprintf(output,100,"%i -> name: %i state: %i hp: %i aistate: %x",i,npcData[i].name,npcData[i].state,npcData[i].hp,(intptr_t)npcData[i].aiState);
		LOG_DEBUG(output);
	}
}
