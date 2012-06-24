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
	int maxy,maxx;

	mathInit();
	logInit();
	inputInit();
	worldInit();
	renderInit();
	playerInit();
	tileInit();
	itemInit();
	npcInit();

	LOG_INFO("Game starting...");
	LOG_INFO("Build time: " __DATE__ " " __TIME__);

	msgAdd("Welcome to " TO_STRING(__GAME_NAME__),TERM_COLOR_RED);

	getmaxyx(stdscr,maxy,maxx);
	noecho();

	if(maxx < 80 || maxy < 24){
		renderUnInit();
		printf("Console size is too small 80x24 is the minimun size!\n");
		return 0;
	}

	/*
	int key;
	while(1){
		key = getch();

		printw("Key %i",key);
	}
	*/

	mapCreateRoom(0);
	playerGetInfo()->pos.z = 0;

	struct pos_s pos =  mapFindTilePos(playerGetInfo()->pos.z,TILE_STAIRS_UP);
	playerGetInfo()->pos.x = pos.x;
	playerGetInfo()->pos.y = pos.y;

	//give the starting itemset
	itemSpawn(playerGetInfo()->pos,ITEM_PISTOL);
	itemSpawn(playerGetInfo()->pos,ITEM_DYNAMITE);
	itemSpawn(playerGetInfo()->pos,ITEM_DYNAMITE);
	itemSpawn(playerGetInfo()->pos,ITEM_DYNAMITE);
	itemSpawn(playerGetInfo()->pos,ITEM_DYNAMITE);
	itemSpawn(playerGetInfo()->pos,ITEM_DYNAMITE);
	itemPickup();

	mapScanFov();

	//==========================================================//
	//  Main loop
	//==========================================================//          
	while(playerGetInfo()->endGame == ENDGAME_REASON_NOTENDED){
		inputHandle();
		npcAiTick();
		itemTick();

		clear();

		mapRender();
		itemRender();
		npcRender();
		playerRender();
		hudRender();

		getmaxyx(stdscr,maxy,maxx);

		if(maxx > 145){
			msgRenderSideView();
		}

		if(maxy > 35){
			debugShowAllocations();
		}
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
			playerAddPoints(itemCountEndBonus());
			msgAdd("You are dead.",TERM_COLOR_RED);

			printw("    _____\n");
			printw("   /     \\ \n");
			printw("  /       \\ \n");
			printw("  | R.I.P  | \n");
			printw("  |        | \n");
			printw("  |        | \n");
			printw("  |        | \n");
			printw("  |________| \n");

			printw("Turns: %i Points: %i\n",playerGetInfo()->turn, playerGetInfo()->points);
			printw("Kills: %i (+ %i innocents)",playerGetInfo()->hostile_kills,playerGetInfo()->innocent_kills);
			printw("\n");
			printw("\n");
			printw("\n");
			playerDumpToFile();
			break;
		case ENDGAME_REASON_WIN:
			playerAddPoints(itemCountEndBonus());
			printw("Victory! Points: %i", playerGetInfo()->points);
			break;
	}

	printw("Press any key to continue...");
	getch();//Wait for input before quitting

	LOG_INFO("Closing game...");

	renderUnInit();
	logUnInit();
	return 0;
}
