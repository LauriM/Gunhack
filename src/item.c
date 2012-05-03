#include "globals.h"
#include <stdio.h>
#include <string.h>
#include "item.h"
#include "math.h"
#include "map.h"
#include "hud.h"
#include "player.h"
#include "render.h"

itemdata_t itemVis [ITEM_MAX_COUNT];
itemdata_t itemData[ITEM_MAX_COUNT];
item_t     itemInfo[ITEM_COUNT];

#define CREATE_ITEM(p_symbol,p_id,p_rarity,p_type,p_name,p_color) itemInfo[p_id].symbol = p_symbol; itemInfo[p_id].itemRarity = p_rarity;itemInfo[p_id].itemType = p_type; itemInfo[p_id].name = TO_STRING(p_name); itemInfo[p_id].itemColor = p_color;

void itemInit(void){
	//--       symbol     id           rarity      type                  name               color
	CREATE_ITEM('*' , ITEM_HP_SMALL    , 70 , ITEM_TYPE_USABLE , "Small health pack" , TERM_COLOR_RED);
	CREATE_ITEM('+' , ITEM_HP_BIG      , 60 , ITEM_TYPE_USABLE , "Large health pack" , TERM_COLOR_RED);
	CREATE_ITEM('/' , ITEM_MELEE_KNIFE , 50 , ITEM_TYPE_MELEE  , "Knife"             , TERM_COLOR_DEFAULT);
	CREATE_ITEM('%' , ITEM_CORPSE      , 5  , ITEM_TYPE_EDIBLE , "Corpse"            , TERM_COLOR_RED);

	//Reset the array
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		itemData[i].state = ITEMSTATE_EMPTY;
		itemVis[i].state  = ITEMSTATE_EMPTY;
	}
}

void itemClearFromLevel(int z){
	ASSERT_ROOM(z);
	
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemVis[i].state == ITEMSTATE_EMPTY)
			continue;
		if(itemVis[i].pos.z != z)
			continue;

		itemVis[i].state = ITEMSTATE_EMPTY;
	}

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state == ITEMSTATE_EMPTY)
			continue;

		if(itemData[i].pos.z != z)
			continue;

		itemData[i].state = ITEMSTATE_EMPTY;//Removes the item from the game
	}
}

void itemVisCreate(pos_t pos,itemtype_t type){
	ASSERT_POS_T(pos);
	ASSERT_ITEM_MAX_COUNT(type);

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemVis[i].state == ITEMSTATE_EMPTY){
			itemVis[i].state  = ITEMSTATE_GROUND;
			itemVis[i].itemId = type;
			itemVis[i].pos    = pos;

			return;
		}
	}
	LOG_ERROR("ITEM_MAX_COUNT too small!");
	return;
}

void itemSpawn(pos_t pos,itemtype_t type){
	ASSERT_POS_T(pos);

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
        if(itemData[i].state == ITEMSTATE_EMPTY){
			itemData[i].state  = ITEMSTATE_GROUND;
			itemData[i].itemId = type;
			itemData[i].pos.x      = pos.x;
			itemData[i].pos.y      = pos.y;
			itemData[i].pos.z      = pos.z;

			return;
		}
	}
	LOG_ERROR("ITEM_MAX_COUNT too small!");
	return;
}

void itemSpawnRandom(int z){
	ASSERT_ROOM(z);
	int i;
	pos_t pos;
	pos.x = 0;
	pos.z = 0;
	pos.y = 0;

	int itemCount = randomRange(GEN_ITEM_COUNT_MIN,GEN_ITEM_COUNT_MAX);

	i = 0;
	while(i < itemCount){
		int done = false;
		while(done == false){
			//TODO: Add fallback if level is full!
			pos.x = randomRange(1,MAP_MAX_WIDTH-1);
			pos.y = randomRange(1,MAP_MAX_HEIGHT-1);

			//TODO: Implement nice rarity generator
			if(mapGetTileByPos(pos)->block == 0){
				int type = randomMax(ITEM_COUNT);
				itemSpawn(pos,type);
				done = true;
			}
		}
		i++;
	}
}

/* Returns data of item in world */
struct itemdata_s* itemGetData(int id){
	ASSERT_ITEM_MAX_COUNT(id);
	return &itemData[id];
}

/* Returns item visual info */ 
struct itemdata_s* itemGetVis(int id){
	ASSERT_ITEM_MAX_COUNT(id);
	return &itemVis[id];
}

/* Returns information about item */
struct item_s* itemGetInfo(int id){
	ASSERT_ITEM_INFO_MAX_COUNT(id);
    return &itemInfo[id];
}

void itemRender(void){
	//Render process
	//1) scan all visData, if seen, remove
	//2) scan all itemData, if seen, add to visData
	//3) render from visData
	
	int playerX,playerY,playerZ;
	playerX = playerGetInfo()->pos.x;
	playerY = playerGetInfo()->pos.y;
	playerZ = playerGetInfo()->pos.z;

	//scan visdata
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemVis[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemVis[i].pos.z != playerZ)
			continue;

		if(mapLosCheck(playerX,playerY,itemVis[i].pos.x,itemVis[i].pos.y) != 1)
			continue;


		itemVis[i].state = ITEMSTATE_EMPTY;//removes the visdata
		LOG_DEBUG("removed");
	}

	//scan itemdata
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemData[i].pos.z != playerZ)
			continue;

		if(mapLosCheck(playerX,playerY,itemData[i].pos.x,itemData[i].pos.y) != 1)
			continue;

		//Add to visdata
		pos_t newPos;
		newPos.z = playerZ;
		newPos.x = itemData[i].pos.x;
		newPos.y = itemData[i].pos.y;

		itemVisCreate(newPos,itemData[i].itemId);
		LOG_DEBUG("Added");
	}

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemVis[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemVis[i].pos.z != playerZ)
			continue;
		
		int symbol = itemGetInfo(itemGetVis(i)->itemId)->symbol;

		setColor(itemGetInfo(itemGetVis(i)->itemId)->itemColor);
		printIntxy(itemGetVis(i)->pos.x,itemGetVis(i)->pos.y,symbol);
		setColorOff(itemGetInfo(itemGetVis(i)->itemId)->itemColor);

		LOG_DEBUG("rendered");
	}
}

void itemPickup(){
    //First get count of the items, if many items, ask what to pickup.

    int itemCount = 0;
	int itemId = 0;

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemData[i].pos.x != playerGetInfo()->pos.x || itemData[i].pos.y != playerGetInfo()->pos.y || itemData[i].pos.z != playerGetInfo()->pos.z)
			continue;

		itemCount++;
		itemId = i;
	}

	if(itemCount == 0){
		hudMessage("Nothing to pickup");
		LOG_INFO("nothing to pickup");
		return;
	}

	if(itemCount == 1){
		hudMessage("Picked up item");
		LOG_INFO("Picking up single item");

		itemData[itemId].state = ITEMSTATE_INV;
		return;
	}

	if(itemCount >= 2){
		hudMessage("choose item to pickup:");

		LOG_WARNING("Picking up multitiple items not implemented!");
		return;
	}
}

extern void itemDisplayInv(){
	hudMenuInit();

	hudMenuWrite("Inventory:");
	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			char output[100];
			snprintf(output,100,"Id: %i Symbol: %c Name: %s",i,itemInfo[itemData[i].itemId].symbol,itemInfo[itemData[i].itemId].name);

			hudMenuWrite(output);
		}
	}
	hudMenuFinish();
}

extern void itemDebugDumpInv(){
	LOG_INFO("Dumping full inventory");

	for(int i = 0;i < ITEM_MAX_COUNT;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			char output[100];

			snprintf(output,100,"Id: %i Symbol: %c Name: %s",i,itemInfo[itemData[i].itemId].symbol,itemInfo[itemData[i].itemId].name);

			LOG_INFO(output);
		}
	}
}
