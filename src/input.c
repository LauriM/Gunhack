#include <ncurses.h>

#include "input.h"
#include "globals.h"
#include "player.h"
#include "math.h"//QQQ
#include "map.h"

void inputHandle(){
	int key = getch();
	switch(key){
		case 60://<
			break;
		case 62://>
			if(room[currentRoom].mapData[playerX][playerY] == TILE_STAIRS_DOWN){
				currentRoom = currentRoom + 1;
				playerRandomPosition();
				mapCreateRoom(currentRoom);//QQQ
			}
			break;
		case 114://r
			mapCheatSeeAll();
			break;
		case 116://t
			playerRandomPosition();
			mapCreateRoom(currentRoom);//QQQ
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
