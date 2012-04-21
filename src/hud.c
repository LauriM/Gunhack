#include <ncurses.h>
#include "globals.h"
#include "hud.h"
#include "render.h"
#include "player.h"
#include <stdio.h>

int prompt(char text[250]){
	move(0,0);

	//TODO: Order of this feels weird, fix the (y/n)
	printw(text);
	//sprintf(text,"%s (y/n)",text);

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

	LOG_WARNING("User failed to answer y/n. Returning n");
	//Not y, return false just to be sure
	return 0;
}

void hudMessage(char text[250]){
	move(0,0);
	printw(text);
}

void hudRender(){
	char statusLine[STATUSLINE_MAX_CHARS] = "";

	sprintf(statusLine,"Level: %i Hp: %i/%i Exp: %i Level: %i",playerGetInfo()->playerZ,playerGetInfo()->hp,playerGetInfo()->maxhp,playerGetInfo()->exp,playerGetInfo()->level);
	
	move(22,0);
	printw(statusLine);
}
