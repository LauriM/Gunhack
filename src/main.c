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
#include "npc.h"

int main(int argc, const char *argv[]){
	logInit();
	worldInit();
	renderInit();
	playerInit();
	tileInit();
	itemInit();
	npcInit();

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
	if(hudPrompt("Load a save from disk?") == 1){
		loadGame();
	}else{
		roomInit(0);
		mapCreateRoom(0);
		playerGetInfo()->pos.z = 0;

		struct pos_s pos =  mapFindTilePos(playerGetInfo()->pos.z,TILE_STAIRS_UP);
		playerGetInfo()->pos.x = pos.x;
		playerGetInfo()->pos.y = pos.y;

		mapScanFov();
	}


	//==========================================================//
	//  Main loop
	//==========================================================//          
	while(playerGetInfo()->endGame == ENDGAME_REASON_NOTENDED){
		npcAiTick();

		mapRender();
		itemRender();
		npcRender();
		playerRender();
		hudRender();

		inputHandle();

		clear();
	}

	//ending game
	clear();

	LOG_INFO("Game over has been triggered.");

	switch(playerGetInfo()->endGame){
		case ENDGAME_REASON_NOTENDED:
			printw("Error!");
			LOG_ERROR("Ending game when game should not end!");
			break;
		case ENDGAME_REASON_SAVE:
			printw("Game saved, see you soon!");
			break;
		case ENDGAME_REASON_GAMEOVER:
			printw("Game over!");
			break;
		case ENDGAME_REASON_WIN:
			printw("Victory!");
			break;
	}

	getch();//Wait for input before quitting

	LOG_INFO("Closing game...");

	renderUnInit();
	logUnInit();
	return 0;
}
