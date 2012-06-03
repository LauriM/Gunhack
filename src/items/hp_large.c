#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"

void itemCall_hp_large(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		msgAdd("You ate large health pack! (+10hp)",TERM_COLOR_GREEN);
		playerGetInfo()->hp = playerGetInfo()->hp + 10;

		if(playerGetInfo()->hp > playerGetInfo()->maxhp)
			playerGetInfo()->hp = playerGetInfo()->maxhp;

		//Destroy the item
		itemData[itemId].state = ITEMSTATE_EMPTY;
	}

	return;
}
