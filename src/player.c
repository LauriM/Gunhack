#include "player.h"
#include "globals.h"
#include "map.h"

void playerMove(int direction){
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
	if(mapGetTileByPos(currentRoom,newX,newY).moveBlock == 0){
		playerX = newX;
		playerY = newY;
	}
}

void playerInit(void){
	currentRoom = 0;
	playerX     = 0;
	playerY     = 0;
}
