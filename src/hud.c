#include <ncurses.h>
#include "globals.h"
#include "hud.h"
#include "render.h"
#include "player.h"
#include <stdio.h>

int hudPrompt(char text[250]){
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
	//TODO: Rendering the message won't work. Some kind of message buffer required(?)
	move(0,0);
	printw(text);
}

void hudRender(){
	char statusLine[STATUSLINE_MAX_CHARS] = "";

	snprintf(statusLine,STATUSLINE_MAX_CHARS,"Level: %i Hp: %i/%i Exp: %i Level: %i Turn: %i",playerGetInfo()->pos.z,playerGetInfo()->hp,playerGetInfo()->maxhp,playerGetInfo()->exp,playerGetInfo()->level,playerGetInfo()->turn);
	
	move(22,0);
	printw(statusLine);
}

int hudMenu(const char* title,char** options,const char* prompt){
	move(0,0);//TODO: check if this call is needed
    printw(title);
	printw("\n");
	

	while(*options != NULL){
		printw(*options);
		printw("\n");
		++options;
	}
	printw("\n");
	
	printw(prompt);

	return getch();
}

void hudMenuInit(){
	move(0,0);
}

void hudMenuWrite(char *line){
	printw(line);
	printw("\n");
}

int hudMenuFinish(){
	return getch();
}
