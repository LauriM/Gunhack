#include "globals.h"
#include "player.h"
#include "render.h"
#include "map.h"
#include "math.h"

struct player_s playerInfo;

void playerInit(){
	//TODO: Why repeat player? why not x 
	playerInfo.playerX = 10;
	playerInfo.playerY = 10;
	playerInfo.hp      = 10;
	playerInfo.points  = 0;
	playerInfo.maxhp   = 10;
	playerInfo.exp     = 0;
	playerInfo.level   = 1;
}

void playerRender(){
	setColor(TERM_COLOR_DEFAULT);
	printxy(playerInfo.playerX,playerInfo.playerY,"@");
}

void playerMove(int direction){
	assert(direction <= 3);
	int newX = playerInfo.playerX;
	int newY = playerInfo.playerY;

	switch(direction){
		case DIR_N:
			newY = playerInfo.playerY - 1;
			break;
		case DIR_S:
			newY = playerInfo.playerY + 1;
			break;
		case DIR_W:
			newX = playerInfo.playerX - 1;
			break;
		case DIR_E:
			newX = playerInfo.playerX + 1;
			break;
	}

	//Check if the new position is ok
	if(mapGetTileByPos(playerGetInfo()->playerZ,newX,newY)->block == 0){
		playerInfo.playerX = newX;
		playerInfo.playerY = newY;
		mapScanFov();
	}
}

void playerRandomPosition(void){
	playerInfo.playerX = (random(MAP_MAX_WIDTH  - 4) + 2);
	playerInfo.playerY = (random(MAP_MAX_HEIGHT - 4) + 2);
}

extern struct player_s* playerGetInfo(void){
	return &playerInfo;
}
