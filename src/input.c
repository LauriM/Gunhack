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

extern int currentRoom;//QQQ

void inputHandle(){
	int key = getch();
	switch(key){
		case 106:  
			//playerMove(DIR_S);
			mapCreateRoom(currentRoom);//QQQ
			break;
		case 107:
			//playerMove(DIR_N);
			currentRoom = currentRoom + 1;//QQQ
			mapCreateRoom(currentRoom);
			break;
		case 108:
			playerMove(DIR_E);
			break;
		case 104:
			playerMove(DIR_W);
			break;
	}

}
