#ifndef ITEM_H
#define ITEM_H

enum{
	ITEMSTATE_GROUND = 0,
	ITEMSTATE_INV
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

extern struct item_s itemInfo[ITEM_COUNT];

void itemInit(void);

#endif
