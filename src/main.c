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
#include "msg.h"

int main(int argc, const char *argv[]){
	logInit();
	worldInit();
	renderInit();
	playerInit();
	tileInit();
	itemInit();
	npcInit();
	msgInit();

	msgAdd("Welcome to __GAME_NAME_",TERM_COLOR_RED);

	LOG_INFO("Game starting...");
	LOG_INFO("Build time: " __DATE__ " " __TIME__);

	msgPrintDebugInfo();

	/*
	int key;
	while(1){
		key = getch();

		printf("%i",key);
	}
	*/


	//TODO: Better management for the save files
	if(hudPrompt("Load a save from disk?") == 0){
		loadGame();
	}else{
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
		inputHandle();
		npcAiTick();
		playerIncTurn();

		clear();

		mapRender();
		itemRender();
		npcRender();
		playerRender();
		hudRender();
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
