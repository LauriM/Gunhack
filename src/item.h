#ifndef ITEM_H
#define ITEM_H

#include "globals.h"
#include "render.h"

typedef enum{
	ITEMACTION_USE = 0,
	ITEMACTION_ATTACK,
	ITEMACTION_WIELD,
	ITEMACTION_TICK,
} itemaction_t;

typedef enum{
	ITEMSTATE_GROUND = 0,
	ITEMSTATE_INV,
	ITEMSTATE_EQ,
	ITEMSTATE_EMPTY
} itemstate_t;

typedef enum{
	SLOT_LEG = 0,
	SLOT_HEAD,
	SLOT_BODY,
	SLOT_HANDS,
	SLOT_AMULET,
	SLOT_WPN,
	SLOT_NULL,
} slot_t;

typedef enum{
	ITEM_HP_SMALL = 0,
	ITEM_HP_BIG,
	ITEM_DYNAMITE,
	ITEM_LANDMINE,
	ITEM_CORPSE,
	ITEM_LVL_POTION,
	ITEM_PISTOL,
	ITEM_AK47,
	//ITEM_SHOTGUN, ALSO uncomment ITEM.c itemRemoveSlot line 440
	ITEM_ROCKET_LAUNCHER,
	ITEM_LAW,
	ITEM_9mm_BOX,
	ITEM_39mm_BOX,
	ITEM_rockets_BOX,
	ITEM_shells_BOX,
	ITEM_WOOD_HELMET,
	ITEM_IRON_HELMET,
	ITEM_PLATINUM_HELMET,
	ITEM_TITAN_HELMET,
	ITEM_DIAMOND_HELMET,
	ITEM_COUNT
} itemname_t;

typedef enum{
	ITEM_TYPE_USABLE = 0,
	ITEM_TYPE_GUN,
	ITEM_TYPE_MELEE,
	ITEM_TYPE_ARMOR,
	ITEM_TYPE_AMMO,
} itemtype_t;

typedef struct item_s{
	char symbol;
	int itemRarity;
	itemtype_t itemType;
	char *name;
	color_t itemColor;
	bool canDrop;
	int maxAmmo;
	int defense;
	slot_t slot;
	void(*itemCall)(int itemId, itemaction_t action);
} item_t;

typedef struct itemdata_s{
	itemname_t itemId;//Links to the item_s in the itemInfo
	itemstate_t state;
	pos_t pos;
	int var1;
} itemdata_t;

extern size_t itemVisSize;   
extern size_t itemVisCapacity;
extern size_t itemDataSize;   
extern size_t itemDataCapacity;

extern itemdata_t *itemVis;
extern itemdata_t *itemData;
extern item_t itemInfo[ITEM_COUNT]; //"database" of the items

//itemData -> itemId -> iteminfo -> item_s -> itemtype/rarity

itemdata_t* itemGetData(int id);
itemdata_t* itemGetVis (int id);
item_t*     itemGetInfo(int id);

void itemInit(void);
void itemSpawn(pos_t pos,itemtype_t type);
void itemVisCreate(pos_t pos,itemtype_t type);
void itemSpawnRandom(int z);
void itemClearFromLevel(int z);

void itemRender(void);
void itemDisplayInv();
void itemDisplayEq();
int itemInvChooseItem();

void itemTick();
void itemFireWpn();
void itemPickup();
void itemDrop(int id);
void itemWield(int id);
void itemRemoveSlot(slot_t slot);
void itemApplyWpn();//Used to reload active eq

int itemGetCarriedCount();
int itemGetCharOnPos(pos_t pos);
int itemGetCurrentAmmoOnWeapon();
int itemGetCurrentMaxAmmoOnWeapon();
int itemGiveCurrentWpnId();

void itemYouSeePos(pos_t pos);

extern int itemGiveRandomDropId();

extern void itemDebugDumpInv();

int itemCountTotalDef();
int itemCountEndBonus();

typedef void (*itemcall_t)(int itemId,itemaction_t action);
void itemCall_null(int itemId,itemaction_t action);
void itemCall_hp_small(int itemId,itemaction_t action);
void itemCall_hp_large(int itemId,itemaction_t action);
void itemCall_potion_gain_level(int itemId,itemaction_t action);
void itemCall_pistol(int itemId,itemaction_t action);
void itemCall_rLauncher(int itemId,itemaction_t action);
void itemCall_law(int itemId,itemaction_t action);
void itemCall_ak47(int itemId,itemaction_t action);
void itemCall_dynamite(int itemId,itemaction_t action);
void itemCall_landmine(int itemId,itemaction_t action);

#endif
