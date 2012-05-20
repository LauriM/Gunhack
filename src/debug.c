#include "globals.h"
#include "render.h"
#include "debug.h"
#include "npc.h"
#include "item.h"
#include "map.h"

#include <ncurses.h>

void debugShowAllocations(){
	move(24,0);
	printw(">Memory statistics\n");

	printw("npcData: %i/%i\n",npcDataSize,npcDataCapacity);
	printw("itemData: %i/%i\n",itemDataSize,itemDataCapacity);
	printw("itemVis: %i/%i\n",itemVisSize,itemVisCapacity);
	printw("room: %i\n",roomSize);
}
