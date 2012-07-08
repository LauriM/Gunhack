#include "globals.h"
#include "npc.h"
#include "map.h"
#include "item.h"
#include <stdbool.h>
#include "player.h"
#include "msg.h"
#include <math.h>

size_t npcDataSize     = 0;
size_t npcDataCapacity = 0;
npcdata_t *npcData     = NULL;

npc_t npcInfo[NPC_COUNT];

#define CREATE_NPC(p_symbol,p_id,p_name,p_color) npcInfo[p_id].symbol = p_symbol; npcInfo[p_id].name = TO_STRING(p_name); npcInfo[p_id].color = p_color; 

void npcInit(void){
	//--    symbol , id                 , name             , color
	CREATE_NPC('x' , NPC_GRID_BUG       , "Grid bug"       , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('g' , NPC_GNOME          , "Gnome"          , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('k' , NPC_KOBOLD         , "Kobold"         , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('G' , NPC_GNOME_LORD     , "Gnome Lord"     , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('g' , NPC_GNOME_SHAMAN   , "Gnome Shaman"   , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('K' , NPC_KOBOLD_LORD    , "Kobold Lord"    , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('k' , NPC_KOBOLD_SHAMAN  , "Kobold Shaman"  , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('w' , NPC_WOLF           , "Wolf"           , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('d' , NPC_DWARF          , "Dwarf"          , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('o' , NPC_ORC            , "Orc"            , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('B' , NPC_BUGBEAR        , "Bugbear"        , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('S' , NPC_SPIDER         , "Giant Spider"   , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('I' , NPC_IMP            , "Imp"            , TERM_COLOR_RED     ) ;
	CREATE_NPC('O' , NPC_ORC_LORD       , "Orc Lord"       , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('O' , NPC_OGRE           , "Ogre"           , TERM_COLOR_RED     ) ;
	CREATE_NPC('O' , NPC_OGRE_LORD      , "Ogre Lord"      , TERM_COLOR_RED     ) ;
	CREATE_NPC('L' , NPC_THUNDER_LIZARD , "Thunder Lizard" , TERM_COLOR_BLUE    ) ;
	CREATE_NPC('G' , NPC_GARGOULE       , "Gargoule"       , TERM_COLOR_GREY    ) ;
	CREATE_NPC('S' , NPC_GIANT_SLIME    , "Giant Slime"    , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('E' , NPC_EFREETI        , "Efreet"         , TERM_COLOR_RED     ) ;
	CREATE_NPC('G' , NPC_GIANT          , "Giant"          , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('C' , NPC_SCORPION       , "Scorpion"       , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('S' , NPC_SNAKE          , "Snake"          , TERM_COLOR_RED     ) ;
	CREATE_NPC('B' , NPC_BEAR           , "Bear"           , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('B' , NPC_GIANT_BEAR     , "Giant Bear"     , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('H' , NPC_HOBO           , "Hobo"           , TERM_COLOR_RED     ) ;
	CREATE_NPC('H' , NPC_DRUNK_HUNTER   , "Drunk Hunter"   , TERM_COLOR_RED     ) ;
	CREATE_NPC('M' , NPC_MERCENARY      , "Mercenary"      , TERM_COLOR_DEFAULT ) ;
	CREATE_NPC('S' , NPC_GIANT_SPIDER   , "Giant SPider"   , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('C' , NPC_CROCODILE      , "Crocodile"      , TERM_COLOR_GREEN   ) ;
	CREATE_NPC('D' , NPC_DINOSAUR       , "Dinosaur"       , TERM_COLOR_GREEN   ) ;

	//CREATE_NPC(,,,,,,,TERM_COLOR_DEFAULT, NPC_RELATION_HOSTILE);

	//Init the npcdata array
	for(int i = 0;i < npcDataSize;i++){
		npcData[i].state   = NPCSTATE_DEAD;
		npcData[i].aiState = &npcState_sleep;
	}
}

void npcSpawnRandom(int z){
	pos_t pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = z;

	//LOG_DEBUG_INT("depth: %i",z);

	int npcCount = randomRange(10,10 + abs(playerGetInfo()->level * 5));

	while(npcCount > 0){
		int done = false;
		while(done == false){
            //TODO: Check for existing enemies

			pos.x = randomRange(1,MAP_MAX_WIDTH-1);
			pos.y = randomRange(1,MAP_MAX_HEIGHT-1);

			if(mapGetTileByPos(pos)->block == 0){
				npcSpawn(pos,randomRange(0,NPC_COUNT-1));
				done = true;
			}
		}
		npcCount--;
	}
}

void npcSpawn(pos_t pos,npcname_t id){
	assert(id < NPC_COUNT);
	assert(id >= 0);

	int i;

	for(i = 0;i < npcDataSize;i++){
		if(npcData[i].state == NPCSTATE_DEAD)
			goto npcSpawnReturn;
	}

	if(npcDataSize >= npcDataCapacity){
		npcDataCapacity *= 2;
		if(npcDataCapacity == 0)
			npcDataCapacity = 2;

		npcData = realloc(npcData,(npcDataCapacity) * sizeof(npcData[0]));
		LOG_DEBUG_F("[mem] Reallocing npcData to %i",npcDataCapacity);
	}

	i = npcDataSize++;

npcSpawnReturn:
	npcData[i].maxHp = (playerGetInfo()->maxhp + randomRange(-10,10));
	npcData[i].maxHp = MAX(10,npcData[i].maxHp);
	npcData[i].meleeDmgMin = (playerGetInfo()->level * randomRange(2,5)); 
	npcData[i].meleeDmgMax = (playerGetInfo()->level * randomRange(5,9));

	npcData[i].meleeDmgMin = MAX(5,npcData[i].meleeDmgMin);
	npcData[i].meleeDmgMax = MAX(10,npcData[i].meleeDmgMax);

	if(randomRange(1,100) > 80){
		npcData[i].relation = NPC_RELATION_PEACEFUL;
	}else{
		npcData[i].relation = NPC_RELATION_HOSTILE;
	}

	npcData[i].state   = NPCSTATE_ALIVE;
	npcData[i].pos     = pos;
	npcData[i].aiState = &npcState_idle;
	npcData[i].name    = id;
	npcData[i].hp      = npcData[i].maxHp;

	LOG_INFO_F("hp: %i/%i min: %i max: %i",npcData[i].hp,npcData[i].maxHp,npcData[i].meleeDmgMin,npcData[i].meleeDmgMax);

	static const pos_t empty = {0,0,0};
	npcData[i].playerLastKnownPosition = empty;
}

void npcRender(){
	for(int i = 0;i < npcDataSize;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		if(npcData[i].pos.z != playerGetInfo()->pos.z)
			continue;

		if(mapLosCheckByPos(playerGetInfo()->pos,npcData[i].pos) == false)
			continue;

		printIntxy(npcData[i].pos.x,npcData[i].pos.y,npcInfo[npcData[i].name].symbol);
	}
}

void npcClearFromLevel(int z){

	for(int i = 0;i < npcDataSize;i++){

		if(npcData[i].pos.z != z)
			continue;

		npcData[i].state = NPCSTATE_DEAD;
	}
}

bool npcExistsInPos(pos_t pos){
	ASSERT_POS_T(pos);

	for(int i = 0;i < npcDataSize;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		if(npcData[i].pos.x == pos.x && npcData[i].pos.y == pos.y && npcData[i].pos.z == pos.z)
			return true;
	}

	return false;
}

bool npcApplyDamagePos(pos_t pos,int damage){
	ASSERT_POS_T(pos);

	for(int i = 0;i < npcDataSize;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		if(npcData[i].pos.x != pos.x || npcData[i].pos.y != pos.y || npcData[i].pos.z != pos.z)
			continue;

		npcData[i].hp = npcData[i].hp - damage;

		int goreCount = 1;

		if(damage > 25)
			goreCount = 8;

		if(damage > 45)
			goreCount = 25;

		//blood
		pos_t bloodPos;
		for(int count = 0; count < goreCount;count++){
			bloodPos = npcData[i].pos;
			bloodPos.x = bloodPos.x + randomRange(-2,2);
			bloodPos.y = bloodPos.y + randomRange(-2,2);
			mapEditColorPoint(bloodPos,TERM_COLOR_RED);
		}

		
		MSG_ADD("you hit %s! (-%i)",TERM_COLOR_DEFAULT,npcInfo[npcData[i].name].name,damage);

		if(npcData[i].hp < 0){
			//DEATH
			npcKillById(i);

			MSG_ADD("%s is dead!",TERM_COLOR_RED,npcInfo[npcData[i].name].name);

			playerAddPoints(npcData[i].maxHp);
			playerAddExp(npcData[i].maxHp);
			playerAddKill(npcData[i].relation);
		}

		return true;//Return true when hit
		break;
	}

	return false;
}

void npcKillById(int id){
	npcData[id].state = NPCSTATE_DEAD;

	itemSpawn(npcData[id].pos,ITEM_CORPSE);

	itemSpawn(npcData[id].pos,itemGiveRandomDropId());

	LOG_INFO("Enemy down!");
}

void npcAiTick(){
	for(int i = 0;i < npcDataSize;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		int flags = IS_NEUTRAL;//Just a default flag

		switch(npcData[i].relation){
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

		if(npcData[i].hp < npcData[i].maxHp){
			flags = flags + IS_DAMAGED;
		}

		if(npcData[i].pos.x == npcData[i].playerLastKnownPosition.x && npcData[i].pos.y == npcData[i].playerLastKnownPosition.y){
			flags = flags + SEARCH_DONE; 
			LOG_DEBUG_F("[ai] %i Search done!",i);
		}

		if(mapLosCheckByPos(playerGetInfo()->pos,npcData[i].pos) == true){
			flags = flags + SEE_PLAYER;
		}

		//==========================================================//
			(*npcData[i].aiState)(i,flags);
		//==========================================================//          
		if(npcData[i].aiState == *npcState_attack){
			npcMoveToPos(i,mapPathfindStep(npcData[i].pos,playerGetInfo()->pos),true);
		}

		if(npcData[i].aiState == *npcState_flee){
			npcMoveToPos(i,mapFindFleePos(npcData[i].pos,playerGetInfo()->pos),true);
		}

		if(npcData[i].aiState == *npcState_idle){

			//move randomly
			pos_t randPos;
			randPos.z = npcData[i].pos.z;
			randPos.x = (npcData[i].pos.x + randomRange(-1,1));
			randPos.y = (npcData[i].pos.y + randomRange(-1,1));

			npcMoveToPos(i,randPos,false);
		}

		if(npcData[i].aiState == *npcState_search){
			pos_t tempPos = mapPathfindStep(npcData[i].pos,npcData[i].playerLastKnownPosition); //TODO: Manage situation where its not possible to get to the player
			npcMoveToPos(i,tempPos,true);
		}

		if(flags & SEE_PLAYER){
			NPC_UPDATE_LAST_KNOWN_POSITION;
		}
	}
}

void npcMoveToPos(int id,pos_t pos,bool allowAttack){
    //Check that the distance isn't longer than one tile.

	if(fabs(npcData[id].pos.x - pos.x) > 1 || fabs(npcData[id].pos.y - pos.y) > 1){
		LOG_ERROR("Npc tries to jump tiles!");
		return;
	}

	if(playerGetInfo()->pos.x == pos.x && playerGetInfo()->pos.y == pos.y){
		if(allowAttack == false)//If attacking is not allowed, don't move at all.
			return;

		int dmg = randomRange(npcData[id].meleeDmgMin,npcData[id].meleeDmgMax);
		playerApplyDmg(dmg);

		//blood
		pos_t bloodPos;
		bloodPos = playerGetInfo()->pos;
		bloodPos.x = bloodPos.x + randomRange(-1,1);
		bloodPos.y = bloodPos.y + randomRange(-1,1);

		mapEditColorPoint(bloodPos,TERM_COLOR_RED);

		MSG_ADD("%s hits you!",TERM_COLOR_DEFAULT,npcInfo[npcData[id].name].name);

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
	for(int i = 0;i < npcDataSize;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		char output[100];

		snprintf(output,100,"%i -> name: %i state: %i hp: %i aistate: %x",i,npcData[i].name,npcData[i].state,npcData[i].hp,(intptr_t)npcData[i].aiState);
		LOG_DEBUG(output);
	}
}

int npcGetCharOnPos(pos_t pos){
	if(!npcExistsInPos(pos))
		return 0;

	for(int i = 0;i < npcDataSize;i++){
		if(npcData[i].state != NPCSTATE_ALIVE)
			continue;

		if(npcData[i].pos.x == pos.x && npcData[i].pos.y == pos.y && npcData[i].pos.z == pos.z)
			return npcInfo[npcData[i].name].symbol;
	}

	LOG_ERROR("This should never happen! The npcData is corrupted.");
	return 0;
}
