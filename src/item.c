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

#define CREATE_ITEM(p_symbol,p_id,p_rarity,p_type,p_name,p_color,p_call,p_drop,p_slot,p_maxAmmo,p_def) itemInfo[p_id].symbol = p_symbol; itemInfo[p_id].itemRarity = p_rarity;itemInfo[p_id].itemType = p_type; itemInfo[p_id].name = TO_STRING(p_name); itemInfo[p_id].itemColor = p_color; itemInfo[p_id].itemCall = p_call; itemInfo[p_id].canDrop = p_drop; itemInfo[p_id].slot = p_slot; itemInfo[p_id].maxAmmo = p_maxAmmo; itemInfo[p_id].defense = p_def;

void itemInit(void){
	//symbol        , id                   , rarity , type             , name                      , color              , action                      , candrop , slot        , maxAmmo , def
	CREATE_ITEM('+' , ITEM_HP_SMALL        , 7      , ITEM_TYPE_USABLE , "Small health pack"       , TERM_COLOR_DEFAULT , &itemCall_hp_small          , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('+' , ITEM_HP_BIG          , 10     , ITEM_TYPE_USABLE , "Large health pack"       , TERM_COLOR_DEFAULT , &itemCall_hp_large          , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('%' , ITEM_CORPSE          , 0      , ITEM_TYPE_USABLE , "Corpse"                  , TERM_COLOR_RED     , &itemCall_null              , false   , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('!' , ITEM_LVL_POTION      , 90     , ITEM_TYPE_USABLE , "Potion of gain level"    , TERM_COLOR_GREEN   , &itemCall_potion_gain_level , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('=' , ITEM_9mm_BOX         , 10     , ITEM_TYPE_AMMO   , "9mm Ammunition"          , TERM_COLOR_BLUE    , &itemCall_null              , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('=' , ITEM_39mm_BOX        , 20     , ITEM_TYPE_AMMO   , "39mm Ammunition"         , TERM_COLOR_BLUE    , &itemCall_null              , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('=' , ITEM_shells_BOX      , 30     , ITEM_TYPE_AMMO   , "Shotgun shells"          , TERM_COLOR_BLUE    , &itemCall_null              , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('=' , ITEM_rockets_BOX     , 30     , ITEM_TYPE_AMMO   , "Rockets"                 , TERM_COLOR_BLUE    , &itemCall_null              , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('(' , ITEM_PISTOL          , 30     , ITEM_TYPE_GUN    , "9mm Pistol"              , TERM_COLOR_DEFAULT , &itemCall_pistol            , true    , SLOT_WPN    , 12      , 0);
	CREATE_ITEM(')' , ITEM_AK47            , 40     , ITEM_TYPE_GUN    , "AK-47"                   , TERM_COLOR_DEFAULT , &itemCall_ak47              , true    , SLOT_WPN    , 30      , 0);
	CREATE_ITEM('/' , ITEM_SHOTGUN         , 30     , ITEM_TYPE_GUN    , "Shotgun"                 , TERM_COLOR_DEFAULT , &itemCall_shotgun           , true    , SLOT_WPN    , 8       , 0);
	CREATE_ITEM('[' , ITEM_ROCKET_LAUNCHER , 10     , ITEM_TYPE_GUN    , "Rocket Launcher"         , TERM_COLOR_GREEN   , &itemCall_rLauncher         , true    , SLOT_WPN    , 1       , 0);
	CREATE_ITEM('{' , ITEM_LAW             , 10     , ITEM_TYPE_GUN    , "Light AntiTank weapon"   , TERM_COLOR_GREEN   , &itemCall_law               , true    , SLOT_WPN    , 1       , 0);
	CREATE_ITEM('_' , ITEM_LANDMINE        , 25     , ITEM_TYPE_USABLE , "Landmine"                , TERM_COLOR_GREEN   , &itemCall_landmine          , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('|' , ITEM_DYNAMITE        , 5      , ITEM_TYPE_USABLE , "Dynamite"                , TERM_COLOR_RED     , &itemCall_dynamite          , true    , SLOT_NULL   , 0       , 0);
	CREATE_ITEM('^' , ITEM_WOOD_HELMET     , 10     , ITEM_TYPE_ARMOR  , "Wooden Helmet (lvl 1)"   , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HEAD   , 0       , 2);
	CREATE_ITEM('^' , ITEM_IRON_HELMET     , 30     , ITEM_TYPE_ARMOR  , "Iron Helmet (lvl 2)"     , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HEAD   , 0       , 8);
	CREATE_ITEM('^' , ITEM_PLATINUM_HELMET , 60     , ITEM_TYPE_ARMOR  , "Platinum Helmet (lvl 3)" , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HEAD   , 0       , 15);
	CREATE_ITEM('^' , ITEM_TITAN_HELMET    , 70     , ITEM_TYPE_ARMOR  , "Titan Helmet (lvl 4)"    , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HEAD   , 0       , 25);
	CREATE_ITEM('^' , ITEM_DIAMOND_HELMET  , 90     , ITEM_TYPE_ARMOR  , "Diamond Helmet (lvl 5)"  , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HEAD   , 0       , 40);
	CREATE_ITEM(']' , ITEM_WOOD_ARMOR      , 10     , ITEM_TYPE_ARMOR  , "Wood Armor (lvl 1)"      , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_BODY   , 0       , 4);
	CREATE_ITEM(']' , ITEM_IRON_ARMOR      , 44     , ITEM_TYPE_ARMOR  , "Iron Armor (lvl 2)"      , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_BODY   , 0       , 8);
	CREATE_ITEM(']' , ITEM_PLATINUM_ARMOR  , 70     , ITEM_TYPE_ARMOR  , "Platinum Armor (lvl 3)"  , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_BODY   , 0       , 15);
	CREATE_ITEM(']' , ITEM_TITAN_ARMOR     , 80     , ITEM_TYPE_ARMOR  , "Titan Armor (lvl 4)"     , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_BODY   , 0       , 28);
	CREATE_ITEM(']' , ITEM_DIAMOND_ARMOR   , 95     , ITEM_TYPE_ARMOR  , "Diamond Armor (lvl 5)"   , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_BODY   , 0       , 65);
	CREATE_ITEM('-' , ITEM_WOOD_GLOVES     , 20     , ITEM_TYPE_ARMOR  , "Wood Gloves (lvl 1)"     , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HANDS  , 0       , 8);
	CREATE_ITEM('-' , ITEM_IRON_GLOVES     , 40     , ITEM_TYPE_ARMOR  , "Iron Gloves (lvl 2)"     , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HANDS  , 0       , 15);
	CREATE_ITEM('-' , ITEM_PLATINUM_GLOVES , 70     , ITEM_TYPE_ARMOR  , "Platinum Gloves (lvl 3)" , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HANDS  , 0       , 25);
	CREATE_ITEM('-' , ITEM_TITAN_GLOVES    , 80     , ITEM_TYPE_ARMOR  , "Titan Gloves (lvl 4)"    , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HANDS  , 0       , 35);
	CREATE_ITEM('-' , ITEM_DIAMOND_GLOVES  , 90     , ITEM_TYPE_ARMOR  , "Diamond Gloves (lvl 5)"  , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_HANDS  , 0       , 55);
	CREATE_ITEM('*' , ITEM_REGEN_AMULET    , 90     , ITEM_TYPE_ARMOR  , "Amulet of Regeneration"  , TERM_COLOR_GREEN   , &itemCall_regen_amulet      , true    , SLOT_AMULET , 0       , 5);
	CREATE_ITEM('*' , ITEM_DEF_AMULET      , 70     , ITEM_TYPE_ARMOR  , "Amulet of Defense"       , TERM_COLOR_DEFAULT , &itemCall_null              , true    , SLOT_AMULET , 0       , 25);
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
	itemVis[i].var1   = 0;
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
	itemData[i].var1   = 0;
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
	if(itemGetCarriedCount() > PLAYER_MAX_CARRY){
		msgAdd("Cannot carry more!",TERM_COLOR_RED);
		return;
	}

    //First get count of the items, if many items, ask what to pickup.
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemData[i].pos.x != playerGetInfo()->pos.x || itemData[i].pos.y != playerGetInfo()->pos.y || itemData[i].pos.z != playerGetInfo()->pos.z)
			continue;

		if(itemData[i].itemId == ITEM_CORPSE)
			continue; //Don't pickup corpses

		if(itemInfo[itemData[i].itemId].itemType == ITEM_TYPE_AMMO){
			//Its ammunition..
			if(itemData[i].itemId == ITEM_9mm_BOX){
				playerGetInfo()->ammo_9mm = playerGetInfo()->ammo_9mm + 12;
				msgAdd("Found 9mm rounds. (+12)",TERM_COLOR_GREEN);
				playerAddPoints(1);
			}
			if(itemData[i].itemId == ITEM_39mm_BOX){
				playerGetInfo()->ammo_39mm = playerGetInfo()->ammo_39mm + 30;
				msgAdd("Found 7.62x39mm rounds. (+30)",TERM_COLOR_GREEN);
				playerAddPoints(3);
			}
			if(itemData[i].itemId == ITEM_shells_BOX){
				playerGetInfo()->ammo_shell = playerGetInfo()->ammo_shell + 8;
				msgAdd("Found shotgun shells. (+8)",TERM_COLOR_GREEN);
				playerAddPoints(5);
			}
			if(itemData[i].itemId == ITEM_rockets_BOX){
				playerGetInfo()->ammo_rockets = playerGetInfo()->ammo_rockets + 3;
				msgAdd("Found rockets. (+3)",TERM_COLOR_GREEN);
				playerAddPoints(15);
			}
			itemData[i].state = ITEMSTATE_EMPTY;
		}else{
			//normal pickup happens!
			MSG_ADD("Picked up %s",TERM_COLOR_DEFAULT,itemInfo[itemData[i].itemId].name);
			itemData[i].state = ITEMSTATE_INV;
		}
	}
}

void itemDisplayInv(){
	hudMenuInit();

	hudMenuWrite("Inventory:");

	int hits = 0;
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			char output[100];
			snprintf(output,100,"%i Symbol: %c Name: %s",hits,itemInfo[itemData[i].itemId].symbol,itemInfo[itemData[i].itemId].name);
			++hits;

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
			char *slot = "null";

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

	int hits = 0;

	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			char output[100];
			snprintf(output,100,"%i Symbol: %c Name: %s",hits,itemInfo[itemData[i].itemId].symbol,itemInfo[itemData[i].itemId].name);
			++hits;

			hudMenuWrite(output);
		}
	}

	hudMenuWrite("Write item id to use/wield/wear: ");

	echo();
	char str[80];
	getstr(str);
	int id = atoi(str);
	noecho();

	hits = 0;
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			if(hits == id)
				return i;
			++hits;
		}
	}

	return 0;
}

void itemDumpInvToFile(FILE* file){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_INV){
			char output[100];

			snprintf(output,100,"%s\n",itemInfo[itemData[i].itemId].name);

			fprintf(file,"%s",output);
		}
	}

	fprintf(file,"\nEquipment:\n");

	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_EQ){
			char output[100];
			char *slot = "null";

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
					slot = "Hands:";
					break;
				case SLOT_LEG:
					slot = "Legs:";
					break;
				case SLOT_NULL:
					LOG_ERROR("INVALID EQ SLOT TYPE!");
					slot ="Invalid!";
					break;
			}

			snprintf(output,100,"%s %s\n",slot,itemInfo[itemData[i].itemId].name);

			fprintf(file,"%s",output);
		}
	}
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
		if(itemInfo[i].itemRarity < rarity){
			//Can drop!
			possibleCount++;
		}
	}

	int toBeChosen = randomRange(0,possibleCount);

	int count = 0;
	for(int i = 0;i < ITEM_COUNT;i++){
		if(itemInfo[i].itemRarity < rarity){
			if(count == toBeChosen){
				LOG_INFO_F("RANDOM GIVES NAME:%i/%s",i,itemInfo[i].name);
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
	if(itemInfo[itemData[id].itemId].slot == SLOT_NULL){
		msgAdd("You cannot wield that item!",TERM_COLOR_RED);
		return;
	}

	if(itemData[id].state == ITEMSTATE_INV){
		itemRemoveSlot(itemInfo[itemData[id].itemId].slot);
		itemData[id].state = ITEMSTATE_EQ;
		(*itemInfo[itemData[id].itemId].itemCall)(id,ITEMACTION_WIELD);

		if(itemInfo[itemData[id].itemId].itemType == ITEM_TYPE_ARMOR) {
			MSG_ADD("You wear %s.",TERM_COLOR_DEFAULT,itemInfo[itemData[id].itemId].name);
		} else {
			MSG_ADD("You wield %s.",TERM_COLOR_DEFAULT,itemInfo[itemData[id].itemId].name);
		}

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

void itemApplyWpn(){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_EQ)
			continue;

		if(itemInfo[itemData[i].itemId].slot != SLOT_WPN)
			continue;

		//Found wpn slot, apply it (reload)!
		(*itemInfo[itemData[i].itemId].itemCall)(i,ITEMACTION_USE);
		return;
	}

	msgAdd("You are not wielding a weapon!",TERM_COLOR_DEFAULT);
}

int itemGetCarriedCount(){
	int count = 0;

	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_INV)
			continue;

		++count;
	}

	return count;

}

int itemGiveCurrentWpnId(){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_EQ)
			continue;

		if(itemInfo[itemData[i].itemId].slot != SLOT_WPN)
			continue;

		return i;
	}

	return -1;
}

int itemCountTotalDef(){
	int def = 0;

	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_EQ)
			continue;

		def += itemInfo[itemData[i].itemId].defense;
	}

	return def;
}

int itemCountEndBonus(){
	int amount = 0;

	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_EQ)
			continue;

		amount += itemInfo[itemData[i].itemId].itemRarity;
	}

	return amount;
}


void itemTick(){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state == ITEMSTATE_EMPTY)
			continue;

		(*itemInfo[itemData[i].itemId].itemCall)(i,ITEMACTION_TICK);
	}
}

int itemGetCurrentAmmoOnWeapon(){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_EQ)
			continue;

		if(itemInfo[itemData[i].itemId].slot != SLOT_WPN)
			continue;

		return itemData[i].var1;
	}

	return 0;
}

int itemGetCurrentMaxAmmoOnWeapon(){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_EQ)
			continue;

		if(itemInfo[itemData[i].itemId].slot != SLOT_WPN)
			continue;

		return itemInfo[itemData[i].itemId].maxAmmo;
	}

	return 0;
}

void itemYouSeePos(pos_t pos){
	for(int i = 0;i < itemDataSize;i++){
		if(itemData[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemData[i].pos.z != pos.z)
			continue;
		if(itemData[i].pos.y != pos.y)
			continue;
		if(itemData[i].pos.x != pos.x)
			continue;

		MSG_ADD("You see %s here.",TERM_COLOR_DEFAULT,itemInfo[itemData[i].itemId].name);
	}
}

int itemGetCharOnPos(pos_t pos){
	for(int i = itemDataSize;i > 0;--i){
		if(itemData[i].state != ITEMSTATE_GROUND)
			continue;

		if(itemData[i].pos.z != pos.z)
			continue;
		if(itemData[i].pos.y != pos.y)
			continue;
		if(itemData[i].pos.x != pos.x)
			continue;

		return itemInfo[itemData[i].itemId].symbol;
	}
	return 0;
}
