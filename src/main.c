#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#include "render.h"
#include "player.h"
#include "globals.h"
#include "input.h"
#include "map.h"

extern int playerX,playerY;
int key;

int main(int argc, const char *argv[]){
	logInit();
	worldInit();
	renderInit();
	playerInit();
	tileInit();

	srand(time(NULL));

	LOG_INFO("Game starting...");
	LOG_INFO("Build time: " __DATE__ " " __TIME__);

	/*
	while(1){
		key = getch();

		printf("%i",key);
	}
	*/

	//Setup the first level and player
	playerX = 10;
	playerY = 10;

	roomInit(0);
	mapCreateRoom(0);
	currentRoom = 0;
	mapScanFov();

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
