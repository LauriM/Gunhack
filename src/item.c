#include "globals.h"
#include "msg.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include "item.h"
#include "math.h"
#include "map.h"
#include "hud.h"
#include "player.h"
#include "render.h"


size_t itemVisSize      = 0;
size_t itemVisCapacity  = 0;
itemdata_t *itemVis     = NULL;

size_t itemDataSize     = 0;
size_t itemDataCapacity = 0;
itemdata_t *itemData    = NULL;

item_t     itemInfo[ITEM_COUNT];

#define CREATE_ITEM(p_symbol,p_id,p_rarity,p_type,p_name,p_color,p_call,p_drop,p_slot) itemInfo[p_id].symbol = p_symbol; itemInfo[p_id].itemRarity = p_rarity;itemInfo[p_id].itemType = p_type; itemInfo[p_id].name = TO_STRING(p_name); itemInfo[p_id].itemColor = p_color; itemInfo[p_id].itemCall = p_call; itemInfo[p_id].canDrop = p_drop; itemInfo[p_id].slot = p_slot;

void itemInit(void){
	//symbol        , id               , rarity , type             , name                   , color              , action                      , candrop , slot
	CREATE_ITEM('*' , ITEM_HP_SMALL    , 70     , ITEM_TYPE_USABLE , "Small health pack"    , TERM_COLOR_DEFAULT , &itemCall_hp_small          , true    , SLOT_NULL);
	CREATE_ITEM('+' , ITEM_HP_BIG      , 70     , ITEM_TYPE_USABLE , "Large health pack"    , TERM_COLOR_DEFAULT , &itemCall_hp_large          , true    , SLOT_NULL);
	CREATE_ITEM('/' , ITEM_MELEE_KNIFE , 70     , ITEM_TYPE_MELEE  , "Knife"                , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_WPN);
	CREATE_ITEM('%' , ITEM_CORPSE      , 0      , ITEM_TYPE_USABLE , "Corpse"               , TERM_COLOR_RED     , &itemCall_null              , false   , SLOT_NULL);
	CREATE_ITEM('!' , ITEM_LVL_POTION  , 2      , ITEM_TYPE_USABLE , "Potion of gain level" , TERM_COLOR_GREEN   , &itemCall_potion_gain_level , true    , SLOT_NULL);
	CREATE_ITEM('(' , ITEM_PISTOL      , 5      , ITEM_TYPE_GUN    , "9mm Pistol"           , TERM_COLOR_DEFAULT , &itemCall_pistol            , true    , SLOT_WPN);
}

void itemClearFromLevel(int z){
	//TODO: FIX
	for(int i = 0;i < itemVisSize;i++){
		if(itemVis[i].state != ITEMSTATE_GROUND)
			continue;
		if(itemVis[i].pos.z != z)
			continue;

		itemVis[i].state = ITEMSTATE_EMPTY;
	}

	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemData[i].pos.z != z)
			continue;

		itemData[i].state = ITEMSTATE_EMPTY;//Removes the item from the game
	}
}

void itemVisCreate(pos_t pos,itemtype_t type){
	ASSERT_POS_T(pos);

	int i;

	for(i = 0;i < itemVisCapacity;i++){
		if(itemVis[i].state == ITEMSTATE_EMPTY){
			goto visSpawnReturn;
		}
	}

	if(itemVisSize >= itemVisCapacity){
		itemVisCapacity *= 2;
		if(itemVisCapacity == 0)
			itemVisCapacity = 2;

		itemVis = realloc(itemVis,(itemVisCapacity)*sizeof(itemVis[0]));
		LOG_DEBUG_F("[mem] Reallocing itemVis to %i",itemVisCapacity);
	}

	i = itemVisSize++;

visSpawnReturn:
	itemVis[i].state  = ITEMSTATE_GROUND;
	itemVis[i].itemId = type;
	itemVis[i].pos    = pos;
	return;
}

void itemSpawn(pos_t pos,itemtype_t type){
	ASSERT_POS_T(pos);
	int i;

	for(i = 0;i < itemDataSize;i++){
        if(itemData[i].state == ITEMSTATE_EMPTY)
			goto itemSpawnReturn;
	}

	if(itemDataSize >= itemDataCapacity){
		itemDataCapacity *= 2;
		if(itemDataCapacity == 0)
            itemDataCapacity = 2;

		itemData = realloc(itemData,(itemDataCapacity) * sizeof(itemData[0]));
		LOG_DEBUG_F("[mem] Reallocing itemData to %i",itemDataCapacity);
	}

	i = itemDataSize++;

itemSpawnReturn:
	itemData[i].state  = ITEMSTATE_GROUND;
	itemData[i].itemId = type;
	itemData[i].pos.x  = pos.x;
	itemData[i].pos.y  = pos.y;
	itemData[i].pos.z  = pos.z;
}

void itemSpawnRandom(int z){
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
				itemSpawn(pos,itemGiveRandomDropId());
				done = true;
			}
		}
		i++;
	}
}

/* Returns data of item in world */
struct itemdata_s* itemGetData(int id){
	return &itemData[id];
}

/* Returns item visual info */ 
struct itemdata_s* itemGetVis(int id){
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
	for(int i = 0;i < itemVisSize;i++){
		if(itemVis[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemVis[i].pos.z != playerZ)
			continue;

		if(mapLosCheck(playerX,playerY,itemVis[i].pos.x,itemVis[i].pos.y) != 1)
			continue;


		itemVis[i].state = ITEMSTATE_EMPTY;//removes the visdata
	}

	//scan itemdata
	for(int i = 0;i < itemDataSize;i++){
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
	}

	for(int i = 0;i < itemVisSize;i++){
		if(itemVis[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemVis[i].pos.z != playerZ)
			continue;
		
		int symbol = itemGetInfo(itemGetVis(i)->itemId)->symbol;

		setColor(itemGetInfo(itemGetVis(i)->itemId)->itemColor);
		printIntxy(itemGetVis(i)->pos.x,itemGetVis(i)->pos.y,symbol);
		setColorOff(itemGetInfo(itemGetVis(i)->itemId)->itemColor);
	}
}

void itemPickup(){
    //First get count of the items, if many items, ask what to pickup.

	int itemCount = 0;

	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemData[i].pos.x != playerGetInfo()->pos.x || itemData[i].pos.y != playerGetInfo()->pos.y || itemData[i].pos.z != playerGetInfo()->pos.z)
			continue;

		//Pickup happens!
		MSG_ADD("Picked up %s",TERM_COLOR_DEFAULT,itemInfo[itemData[i].itemId].name);
		itemData[i].state = ITEMSTATE_INV;
		itemCount++;
	}

	if(itemCount == 0){
		msgAdd("Nothing to pickup!",TERM_COLOR_DEFAULT);
	}
	if(itemCount > 1){
		MSG_ADD("Picked up %i items.",TERM_COLOR_DEFAULT,itemCount);
		return;
	}
}

void itemDisplayInv(){
	hudMenuInit();

	hudMenuWrite("Inventory:");
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			char output[100];
			snprintf(output,100,"Id: %i Symbol: %c Name: %s",i,itemInfo[itemData[i].itemId].symbol,itemInfo[itemData[i].itemId].name);

			hudMenuWrite(output);
		}
	}
	hudMenuFinish();
}

void itemDisplayEq(){
	hudMenuInit();

	hudMenuWrite("Equipment:");
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_EQ){
			char output[100];
			char *slot;

			switch(itemInfo[itemData[i].itemId].slot){
				case SLOT_HEAD:
					slot = "Head:";
					break;
				case SLOT_WPN:
					slot = "Weapon:";
					break;
				case SLOT_BODY:
					slot = "Body:";
					break;
				case SLOT_AMULET:
					slot = "Amulet:";
					break;
				case SLOT_HANDS:
					slot = "Hands";
					break;
				case SLOT_LEG:
					slot = "Legs:";
					break;
				case SLOT_NULL:
					LOG_ERROR("INVALID EQ SLOT TYPE!");
					slot ="Invalid!";
					break;
			}

			snprintf(output,100,"%s %s",slot,itemInfo[itemData[i].itemId].name);

			hudMenuWrite(output);
		}
	}
	hudMenuFinish();
}

int itemInvChooseItem(){
	hudMenuInit();

	hudMenuWrite("Inventory:");
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			char output[100];
			snprintf(output,100,"Id: %i Symbol: %c Name: %s",i,itemInfo[itemData[i].itemId].symbol,itemInfo[itemData[i].itemId].name);

			hudMenuWrite(output);
		}
	}

	hudMenuWrite("Write item id to use: ");

	char str[80];
	getstr(str);
	return atoi(str);
}

void itemDebugDumpInv(){
	LOG_INFO("Dumping full inventory");

	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			char output[100];

			snprintf(output,100,"Id: %i Symbol: %c Name: %s",i,itemInfo[itemData[i].itemId].symbol,itemInfo[itemData[i].itemId].name);

			LOG_INFO(output);
		}
	}
}

int itemGiveRandomDropId(){
	int rarity        = randomRange(1,100);
	int possibleCount = 0;

	for(int i = 0;i < ITEM_COUNT;i++){
		if(itemInfo[i].itemRarity > rarity){
			//Can drop!
			possibleCount++;
		}
	}

	int toBeChosen = randomRange(0,possibleCount);

	int count = 0;
	for(int i = 0;i < ITEM_COUNT;i++){
		if(itemInfo[i].itemRarity < rarity){
			if(count == toBeChosen){
				return i;
			}

			count++;
		}
	}

	//Should not be triggered
	return 0;
}

void itemDrop(int id){
	if(itemData[id].state == ITEMSTATE_INV){
		itemData[id].pos = playerGetInfo()->pos;
		itemData[id].state = ITEMSTATE_GROUND;
		MSG_ADD("Dropped item %s.",TERM_COLOR_DEFAULT,itemInfo[itemData[id].itemId].name);
	}
}

void itemWield(int id){
	if(itemData[id].state == ITEMSTATE_INV){
		itemRemoveSlot(itemInfo[itemData[id].itemId].slot);
		itemData[id].state = ITEMSTATE_EQ;
		MSG_ADD("You wield %s.",TERM_COLOR_DEFAULT,itemInfo[itemData[id].itemId].name);
	}
}

void itemRemoveSlot(slot_t slot){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_EQ)
			continue;

		if(itemInfo[itemData[i].itemId].slot != slot)
			continue;

		//Found the slot on the position, remove it
		itemData[i].state = ITEMSTATE_INV;

		MSG_ADD("You remove %s.",TERM_COLOR_DEFAULT,itemInfo[itemData[i].itemId].name);

		return;
	}
	msgAdd("Nothing to remove!",TERM_COLOR_DEFAULT);
}

void itemFireWpn(){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_EQ)
			continue;

		if(itemInfo[itemData[i].itemId].slot != SLOT_WPN)
			continue;

		//Found wpn slot, fire it!
		(*itemInfo[itemData[i].itemId].itemCall)(i,ITEMACTION_ATTACK);
		return;
	}

	msgAdd("You are not wielding a weapon!",TERM_COLOR_DEFAULT);
}
