#include "globals.h"
#include "item.h"
#include "math.h"
#include "map.h"
#include "player.h"
#include "render.h"

struct item_s itemInfo[ITEM_COUNT];
struct itemdata_s itemData[ITEM_MAX_COUNT];

#define CREATE_ITEM(p_symbol,p_id,p_rarity,p_type) itemInfo[p_id].symbol = p_symbol; itemInfo[p_id].itemRarity = p_rarity;itemInfo[p_id].itemType = p_type;

void itemInit(void){
	CREATE_ITEM('+',ITEM_HP_SMALL,70,ITEM_TYPE_USABLE);
	CREATE_ITEM('+',ITEM_HP_BIG,60,ITEM_TYPE_USABLE);

	//Reset the array
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		itemData[i].state = ITEMSTATE_EMPTY;
	}
}

void itemClearFromLevel(int z){
	//TODO: Add assert
	
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state != ITEMSTATE_EMPTY){
			if(itemData[i].z == z){
				itemData[i].state = ITEMSTATE_EMPTY;//Removes the item from the game
			}
		}
	}
}

void itemSpawn(int z,int x,int y,int type){
	//TODO: Add asserts here!
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
        if(itemData[i].state == ITEMSTATE_EMPTY){
			itemData[i].state  = ITEMSTATE_GROUND;
			itemData[i].itemId = type;
			itemData[i].x      = x;
			itemData[i].y      = y;
			itemData[i].z      = z;

			return;
		}
	}
	//TODO: Check error here, for some reason item spawn random
	//      hits this function too many times
	LOG_ERROR("ITEM_MAX_COUNT too small!");
	return;
}

void itemSpawnRandom(int z){
	//TODO: Add assert here!
	int x,y,i;

	int itemCount = randomRange(GEN_ITEM_COUNT_MIN,GEN_ITEM_COUNT_MAX);

	i = 0;
	while(i < itemCount){
		int done = false;
		while(done == false){
			x = randomRange(1,MAP_MAX_WIDTH-1);
			y = randomRange(1,MAP_MAX_HEIGHT-1);

			//TODO: Implement nice rarity generator
			if(mapGetTileByPos(z,x,y)->block == 0){
				int type = random(ITEM_COUNT);
				itemSpawn(z,x,y,type);
				done = true;
			}
		}
		i++;
	}
}

struct itemdata_s* itemGetData(int id){
	//TODO: Add asserts here
	return &itemData[id];
}

struct item_s* itemGetInfo(int id){
	//TODO: Add asserts here
    return &itemInfo[id];
}

void itemRender(){
	int playerX,playerY;
	playerX = playerGetInfo()->playerX;
	playerY = playerGetInfo()->playerY;

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state != ITEMSTATE_EMPTY){
			if(itemData[i].z == playerGetInfo()->playerZ){
				if(mapLosCheck(playerX,playerY,itemData[i].x,itemData[i].y) == 1){
					int symbol = itemGetInfo(itemGetData(i)->itemId)->symbol;
					printIntxy(itemGetData(i)->x,itemGetData(i)->y,symbol);
				}
			}
		}
	}
}
