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
#include "item.h"
#include "save.h"

int main(int argc, const char *argv[]){
	logInit();
	worldInit();
	renderInit();
	playerInit();
	tileInit();
	itemInit();

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


	//TODO: Better management for the save files
	if(prompt("Load a save from disk?") == 1){
		loadGame();
	}else{
		roomInit(0);
		mapCreateRoom(0);
		playerGetInfo()->playerZ = 0;

		struct pos_s pos =  mapFindTilePos(playerGetInfo()->playerZ,TILE_STAIRS_UP);
		playerGetInfo()->playerX = pos.x;
		playerGetInfo()->playerY = pos.y;

		mapScanFov();
	}


	//==========================================================//
	//  Main loop
	//==========================================================//          
	while(playerGetInfo()->endGame == 0){
		mapRender();
		itemRender();
		playerRender();
		hudRender();

		inputHandle();

		clear();
	}

	//ending game
	clear();

	//TODO: End reason code display with the endGame int
	LOG_INFO("Game over has been triggered.");
	printw("Game over!");
	getch();

	LOG_INFO("Closing game...");

	renderUnInit();
	logUnInit();
	return 0;
}
