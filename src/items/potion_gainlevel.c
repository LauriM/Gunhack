#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"

void itemCall_potion_gain_level(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		msgAdd("You drank potion of gain level! You feel stronger!",TERM_COLOR_GREEN);
		playerGetInfo()->level = playerGetInfo()->level + 1;

		//Destroy the item
		itemData[itemId].state = ITEMSTATE_EMPTY;
	}

	return;
}
