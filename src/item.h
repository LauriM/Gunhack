#ifndef ITEM_H
#define ITEM_H
#include "globals.h"

typedef enum{
	ITEMSTATE_GROUND = 0,
	ITEMSTATE_INV,
	ITEMSTATE_EMPTY
} itemstate_t;

typedef enum{
	ITEM_HP_SMALL = 0,
	ITEM_HP_BIG,
	ITEM_COUNT
} itemname_t;

typedef enum{
	ITEM_TYPE_USABLE = 0,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_ARMOR
} itemtype_t;

typedef struct item_s{
	char symbol;
	int itemRarity;
	int itemType;//Wpn/armor
	char *name;
} item_t;

typedef struct itemdata_s{
	itemname_t itemId;//Links to the item_s in the itemInfo
	itemstate_t state;
	int x;
	int y;
	int z;
} itemdata_t;

extern itemdata_t itemVis[ITEM_MAX_COUNT];
extern itemdata_t itemData[ITEM_MAX_COUNT];
extern item_t itemInfo[ITEM_COUNT]; //"database" of the items

//itemData -> itemId -> iteminfo -> item_s -> itemtype/rarity

itemdata_t* itemGetData(int id);
itemdata_t* itemGetVis (int id);
item_t*     itemGetInfo(int id);

extern void itemInit(void);
extern void itemSpawn(int z,int x,int y,int type);
extern void itemVisCreate(int z,int x,int y,int type);
extern void itemSpawnRandom(int z);
extern void itemClearFromLevel(int z);

extern void itemRender(void);
extern void itemDisplayInv();

extern void itemPickup();

extern void itemDebugDumpInv();
#endif
