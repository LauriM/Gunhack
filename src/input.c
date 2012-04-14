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
			if(room[currentRoom].mapData[playerX][playerY] == TILE_STAIRS_UP){
				if(currentRoom == 0){
                    //TODO: can't go into -1 of room table, implement quit message style nethack
				}else{
					currentRoom = currentRoom - 1;
					if(room[currentRoom].roomType == ROOM_TYPE_UNINITIALIZED){
						playerRandomPosition();
						mapCreateRoom(currentRoom);//TODO: Only create if doesn't exist!
					}else{
						//TODO: Locate up stairs and place player there
					}
				}
			}
			break;
		case 62://>
			if(room[currentRoom].mapData[playerX][playerY] == TILE_STAIRS_DOWN){
				if(currentRoom > WORLD_ROOM_COUNT){
					//TODO: can't go into -1 of room table, implement message
				}else{
					currentRoom = currentRoom + 1;
					if(room[currentRoom].roomType == ROOM_TYPE_UNINITIALIZED){
						playerRandomPosition();
						mapCreateRoom(currentRoom);
					}else{
						//TODO: Locate up stairs and place player there
					}
				}
			}
			break;
		case 114://r
			mapCheatSeeAll();
			break;
		case 116://t
			playerRandomPosition();//TODO: Change cheat behavior to drop down level, not to destroy existing levels
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
