#include "globals.h"
#include "player.h"
#include "render.h"
#include "map.h"
#include "math.h"
#include "npc.h"
#include "msg.h"
#include "item.h"
#include <time.h>

player_t playerInfo;

void playerInit(){
	//TODO: Why repeat player? why not x 
	playerInfo.pos.x          = 10;
	playerInfo.pos.y          = 10;
	playerInfo.pos.z          = 0;
	playerInfo.hp             = 10;
	playerInfo.points         = 0;
	playerInfo.maxhp          = 10;
	playerInfo.exp            = 0;
	playerInfo.level          = 1;
	playerInfo.innocent_kills = 0;
	playerInfo.hostile_kills  = 0;

	playerInfo.ammo_rockets   = 0;
	playerInfo.ammo_shell     = 0;
	playerInfo.ammo_39mm      = 30;
	playerInfo.ammo_9mm       = 24;
}

void playerRender(){
	setColor(TERM_COLOR_DEFAULT);
	printxy(playerInfo.pos.x,playerInfo.pos.y,"@");
}

void playerMove(int direction){
	assert(direction <= DIR_COUNT);
	int newX = playerInfo.pos.x;
	int newY = playerInfo.pos.y;

	switch(direction){
		case DIR_N:
			newY = playerInfo.pos.y - 1;
			break;
		case DIR_S:
			newY = playerInfo.pos.y + 1;
			break;
		case DIR_W:
			newX = playerInfo.pos.x - 1;
			break;
		case DIR_E:
			newX = playerInfo.pos.x + 1;
			break;
		case DIR_NW:
			newY = playerInfo.pos.y - 1;
			newX = playerInfo.pos.x - 1;
			break;
		case DIR_NE:
			newX = playerInfo.pos.x + 1;
			newY = playerInfo.pos.y - 1;
			break;
		case DIR_SW:
			newX = playerInfo.pos.x - 1;
			newY = playerInfo.pos.y + 1;
			break;
		case DIR_SE:
			newY = playerInfo.pos.y + 1;
			newX = playerInfo.pos.x + 1;
			break;
	}

	//Check if the new position is ok
	pos_t posToCheck;
	posToCheck.z = playerGetInfo()->pos.z;
	posToCheck.x = newX;
	posToCheck.y = newY;

	if(npcExistsInPos(posToCheck) == 1){ 
		//Attacking npc!

		npcApplyDamagePos(posToCheck,5);

		//movement canceled because of attack
		return;
	}
	if(mapGetTileByPos(posToCheck)->block == 1){
		//Movement blocked, leave function before execution.
		return;
	}

	playerInfo.pos.x = newX;
	playerInfo.pos.y = newY;
	itemYouSeePos(playerInfo.pos);
}

void playerRandomPosition(void){
	playerInfo.pos.x = (randomMax(MAP_MAX_WIDTH  - 4) + 2);
	playerInfo.pos.y = (randomMax(MAP_MAX_HEIGHT - 4) + 2);
}

player_t* playerGetInfo(void){
	return &playerInfo;
}

void playerIncTurn(void){
	//Also check the hp
	
	//TODO: ENALBE THIS on final
	if(playerInfo.hp < 1){
		playerInfo.endGame = ENDGAME_REASON_GAMEOVER;
	}

	if(playerInfo.turn % 2){
		playerInfo.hp += 1;

		if(playerInfo.hp > playerInfo.maxhp)
			playerInfo.hp = playerInfo.maxhp;
	}

	playerInfo.turn++;
}

void playerAddPoints(int amount){
	playerInfo.points += amount;
}

void playerAddExp(int amount){
    playerInfo.exp += amount;

	while(playerInfo.exp > (playerInfo.level * 15)){
		playerInfo.exp -= (playerInfo.level * 15);
		//TODO: Fix that player will lose the exp that goes over the levelup
		++playerInfo.level;
		playerInfo.maxhp += 10;
		MSG_ADD("Welcome to level %i!",TERM_COLOR_GREEN,playerInfo.level);
		playerInfo.hp = playerInfo.maxhp;
	}
}

void playerApplyDmg(int amount){
	playerGetInfo()->hp -= MAX(1,(amount-itemCountTotalDef()));
}

void playerDumpToFile(){
	FILE* file = fopen("dump.txt","w");//TODO: date+time to the file

	fprintf(file,"Player stats\n");
	fprintf(file, "* Points: %i\n",playerInfo.points);
	fprintf(file, "* HP: %i/%i\n",playerInfo.hp,playerInfo.maxhp);
	fprintf(file, "* Exp/Level: %i/%i\n",playerInfo.exp,playerInfo.level);
	fprintf(file, "* Turn: %i\n",playerInfo.turn);
	fprintf(file, "* Dungeon level: %i\n",playerInfo.pos.z);
	fprintf(file, "* Hostile npc killed: %i\n",playerInfo.hostile_kills);
	fprintf(file, "* Innocent npc killed: %i\n",playerInfo.innocent_kills);

	fprintf(file,"\n");

	fprintf(file,"Ammo count\n");
	fprintf(file, "* 9mm %i\n",playerInfo.ammo_9mm);
	fprintf(file, "* 39mm %i\n",playerInfo.ammo_39mm);
	fprintf(file, "* shells %i\n",playerInfo.ammo_shell);
	fprintf(file, "* rockets %i\n",playerInfo.ammo_rockets);

	fprintf(file,"\n");

	mapRenderToFile(file);

	fprintf(file,"\n");
	fprintf(file,"Inventory:\n");
	fprintf(file,"\n");

	itemDumpInvToFile(file);

	fprintf(file,"\n");
	fprintf(file,"Last log messages:\n");
	fprintf(file,"\n");

	msgWriteToFile(file);
	fclose(file);

	file = fopen("rawscores.txt","a");
	fprintf(file,"%ld|%i|%i|%i|%i|%i|%i \n",time(NULL),playerInfo.points,playerInfo.turn,playerInfo.level,playerInfo.pos.z,playerInfo.innocent_kills,playerInfo.hostile_kills);
	fclose(file);

	file = fopen("scores.txt","a");
	fprintf(file,"Unixtime: %ld Points: %i Turn: %i Level: %i Dungeon level:%i Innocent kills: %i Hostile kills: %i\n",time(NULL),playerInfo.points,playerInfo.turn,playerInfo.level,playerInfo.pos.z,playerInfo.innocent_kills,playerInfo.hostile_kills);
	fclose(file);
}


void playerAddKill(int relation){
	if(relation == NPC_RELATION_HOSTILE){
		++playerInfo.hostile_kills;
		return;
	}

	++playerInfo.innocent_kills;
}
