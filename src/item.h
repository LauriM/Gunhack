#ifndef ITEM_H
#define ITEM_H

enum{
	ITEMSTATE_GROUND = 0,
	ITEMSTATE_INV,
	ITEMSTATE_EMPTY
};

enum{
	ITEM_HP_SMALL = 0,
	ITEM_HP_BIG,
	ITEM_COUNT
};

enum{
	ITEM_TYPE_USABLE = 0,
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_ARMOR
};

struct item_s{
	int itemRarity;
	int itemType;
};

struct itemdata_s{
	int itemId;//Links to the item_s in the itemInfo
	int state;
	int x;
	int y;
	int z;
};

extern struct itemdata_s itemData[ITEM_MAX_COUNT];
extern struct item_s itemInfo[ITEM_COUNT];

//itemData -> itemId -> iteminfo -> item_s -> itemtype/rarity

extern void itemInit(void);
extern void itemSpawn(int z,int x,int y,int type);
extern void itemSpawnRandom(int z);

#endif
