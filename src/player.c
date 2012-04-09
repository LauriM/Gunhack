#include "player.h"
#include "globals.h"
#include "map.h"

extern int playerZ;
extern int playerX;
extern int playerY;

extern struct tile_s tileInfo[TILE_COUNT];

void movePlayer(int direction){
	int newX,newY;

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
	/*
	int nextCode = mapGetCodeByxy(playerZ,newX,newY);

	if(mapget)
	}
	*/
}
