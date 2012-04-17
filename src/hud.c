#include <ncurses.h>
#include "globals.h"
#include "hud.h"
#include "render.h"

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
