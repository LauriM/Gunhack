#ifndef ITEM_H
#define ITEM_H

#include "globals.h"
#include "render.h"

typedef enum{
	ITEMACTION_USE = 0,
	ITEMACTION_ATTACK
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
	ITEM_MELEE_KNIFE,
	ITEM_CORPSE,
	ITEM_LVL_POTION,
	ITEM_PISTOL,
	ITEM_ROCKET_LAUNCHER,
	ITEM_9mm_BOX,
	ITEM_39mm_BOX,
	ITEM_rockets_BOX,
	ITEM_shells_BOX,
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
	slot_t slot;
	void(*itemCall)(int itemId, itemaction_t action);
} item_t;

typedef struct itemdata_s{
	itemname_t itemId;//Links to the item_s in the itemInfo
	itemstate_t state;
	pos_t pos;
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

extern void itemInit(void);
extern void itemSpawn(pos_t pos,itemtype_t type);
extern void itemVisCreate(pos_t pos,itemtype_t type);
extern void itemSpawnRandom(int z);
extern void itemClearFromLevel(int z);

extern void itemRender(void);
extern void itemDisplayInv();
extern void itemDisplayEq();
extern int itemInvChooseItem();

extern void itemFireWpn();
extern void itemPickup();
extern void itemDrop(int id);
extern void itemWield(int id);
extern void itemRemoveSlot(slot_t slot);
extern void itemApplyWpn();//Used to reload active eq

extern int itemGiveRandomDropId();

extern void itemDebugDumpInv();

typedef void (*itemcall_t)(int itemId,itemaction_t action);
extern void itemCall_null(int itemId,itemaction_t action);
extern void itemCall_hp_small(int itemId,itemaction_t action);
extern void itemCall_hp_large(int itemId,itemaction_t action);
extern void itemCall_potion_gain_level(int itemId,itemaction_t action);
extern void itemCall_pistol(int itemId,itemaction_t action);
extern void itemCall_rLauncher(int itemId,itemaction_t action);

#endif
