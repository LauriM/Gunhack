#include <ncurses.h>

#include "input.h"
#include "globals.h"
#include "player.h"
#include "math.h"//QQQ
#include "map.h"//QQQ

extern int currentRoom;//QQQ

void inputHandle(){
	int key = getch();
	switch(key){
		case 116://t
			playerX = random(MAP_MAX_WIDTH);
			playerY = random(MAP_MAX_HEIGHT);
			mapCreateRoom(currentRoom);//QQQ
			mapCheatSeeAll();
			break;
		case 106:  
			playerMove(DIR_S);
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
