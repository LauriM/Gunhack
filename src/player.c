#include "globals.h"
#include "player.h"
#include "render.h"
#include "map.h"
#include "math.h"

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
}

void playerRender(){
	setColor(TERM_COLOR_DEFAULT);
	printxy(playerInfo.pos.x,playerInfo.pos.y,"@");
}

void playerMove(int direction){
	assert(direction <= 3);
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

	if(mapGetTileByPos(posToCheck)->block == 0){
		playerInfo.pos.x = newX;
		playerInfo.pos.y = newY;
		mapScanFov();
	}
}

void playerRandomPosition(void){
	playerInfo.pos.x = (randomMax(MAP_MAX_WIDTH  - 4) + 2);
	playerInfo.pos.y = (randomMax(MAP_MAX_HEIGHT - 4) + 2);
}

extern player_t* playerGetInfo(void){
	return &playerInfo;
}
