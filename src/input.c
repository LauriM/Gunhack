#include <ncurses.h>

#include "input.h"
#include "globals.h"
#include "player.h"
#include "map.h"
#include "hud.h"
#include "save.h"
#include "npc.h"
#include "msg.h"

#include "math.h"//QQQ
#include "item.h"

inputcall_t keyBinds[256];

void input_null(void){
	return;
}

void input_move_n(void){
	playerMove(DIR_N);
}

void input_move_s(void){
	playerMove(DIR_S);
}

void input_move_e(void){
	playerMove(DIR_E);
}

void input_move_w(void){
	playerMove(DIR_W);
}

void input_move_nw(void){
	playerMove(DIR_NW);
}

void input_move_ne(void){
	playerMove(DIR_NE);
}

void input_move_sw(void){
	playerMove(DIR_SW);
}

void input_move_se(void){
	playerMove(DIR_SE);
}

void input_applyitem(void){
	int itemIdentifier = itemInvChooseItem();

	if(itemData[itemIdentifier].state == ITEMSTATE_INV){
		(*itemInfo[itemData[itemIdentifier].itemId].itemCall)(itemIdentifier,ITEMACTION_USE);
	}else{
		msgAdd("Invalid item code!",TERM_COLOR_DEFAULT);
	}
}

void input_move_up(void){
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

			playerAddPoints(150);
		}
	}
}

void input_move_down(void){
	if(room[playerGetInfo()->pos.z].mapData[playerGetInfo()->pos.x][playerGetInfo()->pos.y] == TILE_STAIRS_DOWN){
		if(playerGetInfo()->pos.z > roomSize){
			LOG_INFO("Trying to go outside the roomSize!");
		}else{
			playerGetInfo()->pos.z++;
			if(playerGetInfo()->pos.z > roomSize-1){
				playerRandomPosition();
				mapCreateRoom(playerGetInfo()->pos.z);
			}
			struct pos_s pos =  mapFindTilePos(playerGetInfo()->pos.z,TILE_STAIRS_UP);
			playerGetInfo()->pos.x = pos.x;
			playerGetInfo()->pos.y = pos.y;
			mapScanFov();
		}
	}
}

void input_cheat_seeall(void){
	mapCheatSeeAll();
	LOG_INFO("Using see all cheat");
}

void input_cheat_regenlevel(void){
	playerRandomPosition();
	itemClearFromLevel(playerGetInfo()->pos.z);
	npcClearFromLevel(playerGetInfo()->pos.z);
	mapCreateRoom(playerGetInfo()->pos.z);
	LOG_INFO("Forced current room regeneration");
	mapScanFov();
}

void input_save(void){
	saveGame();
}

void input_pickup(void){
	itemPickup();
}

void input_cheat_debuginv(void){
	itemDebugDumpInv();
	npcDumpState();
}

void input_inv(void){
	itemDisplayInv();
}

void input_eq(void){
	itemDisplayEq();
}

void input_wield(void){
	itemWield(itemInvChooseItem());
}

void input_history(void){
	msgShowWindow();
}

void input_drop(void){
	itemDrop(itemInvChooseItem());
}

void input_fire(void){
	itemFireWpn();
}

void input_reload(void){
	itemApplyWpn();
}

void inputInit(){
	for(int i = 0;i < 256;i++){
		keyBinds[i] = &input_null;
	}

	//TODO: Fix keypad keys
	
	keyBinds['<'] = &input_move_up;
	keyBinds['>'] = &input_move_down;

	keyBinds['y'] = &input_cheat_seeall;
	keyBinds['t'] = &input_cheat_regenlevel;
	keyBinds['.'] = &input_cheat_debuginv;

	keyBinds['S'] = &input_save;

	keyBinds[','] = &input_pickup;
	keyBinds['i'] = &input_inv;
	keyBinds['e'] = &input_eq;
	keyBinds['w'] = &input_wield;
	keyBinds['d'] = &input_drop;
	keyBinds['a'] = &input_applyitem;
	keyBinds['f'] = &input_fire;
	keyBinds['r'] = &input_reload;

	keyBinds['l'] = &input_move_e;
	keyBinds['h'] = &input_move_w;
	keyBinds['k'] = &input_move_n;
	keyBinds['j'] = &input_move_s;

	keyBinds[55] = &input_move_nw;
	keyBinds[56] = &input_move_n;
	keyBinds[57] = &input_move_ne;
	keyBinds[52] = &input_move_w;
	keyBinds[54] = &input_move_e;
	keyBinds[49] = &input_move_sw;
	keyBinds[50] = &input_move_s;
	keyBinds[51] = &input_move_se;
}

void inputHandle(){
	while(1){
		int key = getch();
		(*keyBinds[key])();

		if(*keyBinds[key] != &input_null){
			playerIncTurn();
			return;
		}
	}
}

dir_t inputGetDirection(){
	printw("Give a direction: \n");
	int key = getch();

	//Yes, its stupid that these are coded in like this... Whatever
	switch(key){
		case 50:  
			return DIR_S;
			break;
		case 49:
			return DIR_SW;
			break;
		case 51:
			return DIR_SE;
			break;
		case 52:
			return DIR_W;
			break;
		case 54:
			return DIR_E;
			break;
		case 55:
			return DIR_NW;
			break;
		case 56:
			return DIR_N;
			break;
		case 57:
			return DIR_NE;
			break;
	}

	return DIR_S; //Add random dir with msg to player about failure to choose direction
}
