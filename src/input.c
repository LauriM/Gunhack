#include <ncurses.h>

#include "input.h"
#include "globals.h"
#include "player.h"
#include "map.h"
#include "hud.h"
#include "save.h"
#include "npc.h"

#include "math.h"//QQQ
#include "item.h"

void inputHandle(){
	//TODO: Fix the whole fucking input system to dynamic system or something
	int key = getch();
	switch(key){
		case 60://<
			if(room[playerGetInfo()->pos.z].mapData[playerGetInfo()->pos.x][playerGetInfo()->pos.y] == TILE_STAIRS_UP){
				if(playerGetInfo()->pos.z == 0){
					LOG_INFO("Player trying to leave the level by going up");

					if(hudPrompt("Do you want to escape the dungeon?") == 1){
						LOG_INFO("Player left the dungeon!");
						playerGetInfo()->endGame = 1;
					}

				}else{
					playerGetInfo()->pos.z = playerGetInfo()->pos.z - 1;
					if(room[playerGetInfo()->pos.z].roomType == ROOM_TYPE_UNINITIALIZED){
						playerRandomPosition();
						mapCreateRoom(playerGetInfo()->pos.z);
					}
					pos_t pos = mapFindTilePos(playerGetInfo()->pos.z,TILE_STAIRS_DOWN);
					playerGetInfo()->pos.x = pos.x;
					playerGetInfo()->pos.y = pos.y;
					mapScanFov();
				}
			}
			break;
		case 62://>
			if(room[playerGetInfo()->pos.z].mapData[playerGetInfo()->pos.x][playerGetInfo()->pos.y] == TILE_STAIRS_DOWN){
				if(playerGetInfo()->pos.z > WORLD_ROOM_COUNT){
					LOG_INFO("Trying to go outside the world_room_count!");
				}else{
					playerGetInfo()->pos.z = playerGetInfo()->pos.z + 1;
					if(room[playerGetInfo()->pos.z].roomType == ROOM_TYPE_UNINITIALIZED){
						playerRandomPosition();
						mapCreateRoom(playerGetInfo()->pos.z);
					}
					struct pos_s pos =  mapFindTilePos(playerGetInfo()->pos.z,TILE_STAIRS_UP);
					playerGetInfo()->pos.x = pos.x;
					playerGetInfo()->pos.y = pos.y;
					mapScanFov();
				}
			}
			break;
		case 114://r
			mapCheatSeeAll();
			LOG_INFO("Using see all cheat");
			//Cheat
			playerRandomPosition();
			itemClearFromLevel(playerGetInfo()->pos.z);
			npcClearFromLevel(playerGetInfo()->pos.z);
			mapCreateRoom(playerGetInfo()->pos.z);
			LOG_INFO("Forced current room regeneration");
			mapScanFov();
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
		case 83:
			saveGame();
			break;
		case 44:
			itemPickup();
			break;
		case 46: //.
			itemDebugDumpInv();
			break;
		case 105:
			itemDisplayInv();
			npcDumpState();
			break;
	}

	//Numpad:
	// 55 = 7
	// 49 = 1
	// 51 = 3
	// 57 = 9
	// *taken under tmux may or may not be the correct ones*
}
