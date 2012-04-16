#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "render.h"
#include "player.h"
#include "globals.h"
#include "math.h"
#include "input.h"
#include "map.h"

int main(int argc, const char *argv[]){
	logInit();
	worldInit();
	renderInit();
	playerInit();
	tileInit();
	playerInit();

	srand(time(NULL));

	LOG_INFO("Game starting...");
	LOG_INFO("Build time: " __DATE__ " " __TIME__);

	/*
	int key;
	while(1){
		key = getch();

		printf("%i",key);
	}
	*/

	//Setup the level
	roomInit(0);
	mapCreateRoom(0);
	currentRoom = 0;
	mapScanFov();

	struct pos_s pos =  mapFindTilePos(currentRoom,TILE_STAIRS_UP);
	playerGetInfo()->playerX = pos.x;
	playerGetInfo()->playerY = pos.y;

	//==========================================================//
	//  Main loop
	//==========================================================//          
	while(1){
		mapRender();
		playerRender();

		inputHandle();

		clear();
	}

	renderUnInit();
	logUnInit();
	return 0;
}
