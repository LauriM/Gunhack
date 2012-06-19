#include "globals.h"
#include "player.h"
#include "render.h"
#include "map.h"
#include "math.h"
#include "npc.h"
#include "msg.h"
#include "item.h"

player_t playerInfo;

void playerInit(){
	//TODO: Why repeat player? why not x 
	playerInfo.pos.x  = 10;
	playerInfo.pos.y  = 10;
	playerInfo.pos.z  = 0;
	playerInfo.hp     = 10;
	playerInfo.points = 0;
	playerInfo.maxhp  = 10;
	playerInfo.exp    = 0;
	playerInfo.level  = 1;

	playerInfo.ammo_rockets = 0;
	playerInfo.ammo_shell   = 0;
	playerInfo.ammo_39mm    = 30;
	playerInfo.ammo_9mm     = 24;
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
	mapScanFov();
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
	/*
	 *
	if(playerInfo.hp < 1){
		playerInfo.endGame = ENDGAME_REASON_GAMEOVER;
	}
	*/

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

	if(playerInfo.exp > (playerInfo.level * 15)){
		playerInfo.exp = 0;
		//TODO: Fix that player will lose the exp that goes over the levelup
		++playerInfo.level;
		playerInfo.maxhp += 10;
		MSG_ADD("Welcome to level %i!",TERM_COLOR_GREEN,playerInfo.level);
	}
}

void playerApplyDmg(int amount){
	playerGetInfo()->hp -= MAX(1,(amount-itemCountTotalDef()));
}
