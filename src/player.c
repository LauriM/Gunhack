#include "player.h"
#include "globals.h"

extern int playerZ;
extern int playerX;
extern int playerY;

void movePlayer(int direction){
	switch(direction){
		case DIR_N:
			playerY = playerY - 1;
			break;
		case DIR_S:
			playerY = playerY + 1;
			break;
		case DIR_W:
			playerX = playerX - 1;
			break;
		case DIR_E:
			playerX = playerX + 1;
			break;
	}
}
