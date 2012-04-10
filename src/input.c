#include <ncurses.h>

#include "input.h"
#include "globals.h"
#include "player.h"

#include "map.h" //QQQ

//Key codes
//j = 106
//l = 108
//h = 104
//k = 107

void inputHandle(){
	int key = getch();
	switch(key){
		case 106:  
			//playerMove(DIR_S);
			mapCreateRoom(0);//QQQ
			break;
		case 107:
			playerMove(DIR_N);
			break;
		case 108:
			playerMove(DIR_E);
			break;
		case 104:
			playerMove(DIR_W);
			break;
	}

}
