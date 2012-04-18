#include <ncurses.h>
#include "globals.h"
#include "hud.h"
#include "render.h"
#include "player.h"
#include <stdio.h>

int prompt(char text[250]){
	move(0,0);
	printw(text);

	int input = getch();

	//y = 121
	//n = 110
	switch(input){
		case 121:
			return 1;
			break;
		case 110:
			return 0;
			break;
	}

	//Not y, return false just to be sure
	return 0;
}

void hudRender(){
	char statusLine[STATUSLINE_MAX_CHARS] = "";

	sprintf(statusLine,"Depth: %i Hp 23/43",playerGetInfo()->playerZ);
	
	move(22,0);
	printw(statusLine);
}
