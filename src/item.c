#include "globals.h"
#include "item.h"
#include "math.h"
#include "map.h"
#include "player.h"
#include "render.h"

struct itemdata_s itemVis[ITEM_MAX_COUNT];
struct item_s     itemInfo[ITEM_COUNT];
struct itemdata_s itemData[ITEM_MAX_COUNT];

#define CREATE_ITEM(p_symbol,p_id,p_rarity,p_type) itemInfo[p_id].symbol = p_symbol; itemInfo[p_id].itemRarity = p_rarity;itemInfo[p_id].itemType = p_type;

void itemInit(void){
	CREATE_ITEM('*',ITEM_HP_SMALL,70,ITEM_TYPE_USABLE);
	CREATE_ITEM('+',ITEM_HP_BIG,60,ITEM_TYPE_USABLE);

	//Reset the array
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		itemData[i].state = ITEMSTATE_EMPTY;
		itemVis[i].state  = ITEMSTATE_EMPTY;
	}
}

void itemClearFromLevel(int z){
	//TODO: Add assert
	
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemVis[i].state != ITEMSTATE_EMPTY){
			if(itemVis[i].z == z){
				itemVis[i].state = ITEMSTATE_EMPTY;
			}
		}
	}

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state != ITEMSTATE_EMPTY){
			if(itemData[i].z == z){
				itemData[i].state = ITEMSTATE_EMPTY;//Removes the item from the game
			}
		}
	}
}

void itemVisCreate(int z,int x,int y,int type){
	//TODO: Add asserts here!
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemVis[i].state == ITEMSTATE_EMPTY){
			itemVis[i].state  = ITEMSTATE_GROUND;
			itemVis[i].itemId = type;
			itemVis[i].x      = x;
			itemVis[i].y      = y;
			itemVis[i].z      = z;

			return;
		}
	}
	LOG_ERROR("ITEM_MAX_COUNT too small!");
	return;
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

/* Returns data of item in world */
struct itemdata_s* itemGetData(int id){
	//TODO: Add asserts here
	return &itemData[id];
}

/* Returns item visual info */ 
struct itemdata_s* itemGetVis(int id){
	//TODO: Add asserts here
	return &itemVis[id];
}

/* Returns information about item */
struct item_s* itemGetInfo(int id){
	//TODO: Add asserts here
    return &itemInfo[id];
}

void itemRender(void){
	//Render process
	//1) scan all visData, if seen, remove
	//2) scan all itemData, if seen, add to visData
	//3) render from visData
	
	int playerX,playerY;  //TODO: Add playerZ
	playerX = playerGetInfo()->playerX;
	playerY = playerGetInfo()->playerY;

	//scan visdata
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemVis[i].state != ITEMSTATE_EMPTY){
			if(itemVis[i].z == playerGetInfo()->playerZ){
				if(mapLosCheck(playerX,playerY,itemVis[i].x,itemVis[i].y) == 1){
					itemVis[i].state = ITEMSTATE_EMPTY;//removes the visdata
					LOG_DEBUG("removed");
				}
			}
		}
	}

	//scan itemdata
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state != ITEMSTATE_EMPTY){
			if(itemData[i].z == playerGetInfo()->playerZ){
				if(mapLosCheck(playerX,playerY,itemData[i].x,itemData[i].y) == 1){
					//Add to visdata
					itemVisCreate(playerGetInfo()->playerZ,itemData[i].x,itemData[i].y,itemData[i].itemId);
					LOG_DEBUG("Added");
				}
			}
		}
	}

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemVis[i].state == ITEMSTATE_GROUND){
			if(itemVis[i].z == playerGetInfo()->playerZ){
				int symbol = itemGetInfo(itemGetVis(i)->itemId)->symbol;
				printIntxy(itemGetVis(i)->x,itemGetVis(i)->y,symbol);
				LOG_DEBUG("rendered");
			}
		}
	}
}
