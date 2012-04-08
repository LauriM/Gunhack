#include <ncurses.h>

#include "input.h"
#include "globals.h"
#include "player.h"

//Key codes
//j = 106
//l = 108
//h = 104
//k = 107

void inputHandle(){
	int key = getch();
	switch(key){
		case 106:  
			movePlayer(DIR_S);
			break;
		case 107:
			movePlayer(DIR_N);
			break;
		case 108:
			movePlayer(DIR_E);
			break;
		case 104:
			movePlayer(DIR_W);
			break;
	}

}
