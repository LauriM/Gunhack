#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "render.h"
#include "player.h"
#include "globals.h"
#include "hud.h"
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
	playerGetInfo()->playerZ = 0;

	struct pos_s pos =  mapFindTilePos(playerGetInfo()->playerZ,TILE_STAIRS_UP);
	playerGetInfo()->playerX = pos.x;
	playerGetInfo()->playerY = pos.y;

	mapScanFov();

	//==========================================================//
	//  Main loop
	//==========================================================//          
	while(playerGetInfo()->endGame == 0){
		mapRender();
		hudRender();
		playerRender();

		inputHandle();

		clear();
	}

	//ending game
	clear();

	LOG_INFO("Game over has been triggered.");
	printw("Game over!");
	getch();

	LOG_INFO("Closing game...");

	renderUnInit();
	logUnInit();
	return 0;
}
