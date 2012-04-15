#include "globals.h"
#include "player.h"
#include "render.h"
#include "map.h"
#include "math.h"

void playerRender(){
	setColor(TERM_COLOR_DEFAULT);
	printxy(playerX,playerY,"@");
}

void playerMove(int direction){
	assert(direction <= 3);
	int newX = playerX;
	int newY = playerY;

	switch(direction){
		case DIR_N:
			newY = playerY - 1;
			break;
		case DIR_S:
			newY = playerY + 1;
			break;
		case DIR_W:
			newX = playerX - 1;
			break;
		case DIR_E:
			newX = playerX + 1;
			break;
	}

	//Check if the new position is ok
	if(mapGetTileByPos(currentRoom,newX,newY)->block == 0){
		playerX = newX;
		playerY = newY;
		mapScanFov();
	}
}

void playerInit(void){
	currentRoom = 0;
	playerX     = 0;
	playerY     = 0;
}

void playerRandomPosition(void){
	playerX = (random(MAP_MAX_WIDTH  - 4) + 2);
	playerY = (random(MAP_MAX_HEIGHT - 4) + 2);
}
