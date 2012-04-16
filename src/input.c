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
			if(room[currentRoom].mapData[playerGetInfo()->playerX][playerGetInfo()->playerY] == TILE_STAIRS_UP){
				if(currentRoom == 0){
                    //TODO: can't go into -1 of room table, implement quit message style nethack
					LOG_INFO("Player trying to leave the level by going up");
				}else{
					currentRoom = currentRoom - 1;
					if(room[currentRoom].roomType == ROOM_TYPE_UNINITIALIZED){
						playerRandomPosition();
						mapCreateRoom(currentRoom);//TODO: Only create if doesn't exist!
					}else{
						struct pos_s pos =  mapFindTilePos(currentRoom,TILE_STAIRS_DOWN);
						playerGetInfo()->playerX = pos.x;
						playerGetInfo()->playerY = pos.y;
					}
				}
			}
			break;
		case 62://>
			if(room[currentRoom].mapData[playerGetInfo()->playerX][playerGetInfo()->playerY] == TILE_STAIRS_DOWN){
				if(currentRoom > WORLD_ROOM_COUNT){
					//TODO: can't go into -1 of room table, implement message
					LOG_INFO("Trying to go outside the world_room_count!");
				}else{
					currentRoom = currentRoom + 1;
					if(room[currentRoom].roomType == ROOM_TYPE_UNINITIALIZED){
						playerRandomPosition();
						mapCreateRoom(currentRoom);
					}else{
						struct pos_s pos =  mapFindTilePos(currentRoom,TILE_STAIRS_UP);
						playerGetInfo()->playerX = pos.x;
						playerGetInfo()->playerY = pos.y;
					}
				}
			}
			break;
		case 114://r
			mapCheatSeeAll();
			LOG_INFO("Using see all cheat");
			break;
		case 116://t
			playerRandomPosition();//TODO: Change cheat behavior to drop down level, not to destroy existing levels
			mapCreateRoom(currentRoom);//QQQ
			LOG_INFO("Forced room regeneration");
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
