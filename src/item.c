#include "globals.h"
#include "item.h"

struct item_s itemInfo[ITEM_COUNT];

#define CREATE_ITEM(type,rarity,itype) itemInfo[ITEM_HP_SMALL].itemRarity = rarity;itemInfo[ITEM_HP_BIG].itemType = itype;


void itemInit(void){
	CREATE_ITEM(ITEM_HP_SMALL,70,ITEM_TYPE_USABLE);
	CREATE_ITEM(ITEM_HP_BIG,60,ITEM_TYPE_USABLE);
}
