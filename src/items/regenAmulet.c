#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"

void itemCall_regen_amulet(int itemId,itemaction_t action){
	if(action == ITEMACTION_TICK){
		if(itemData[itemId].state == ITEMSTATE_EQ){
			playerGetInfo()->hp = playerGetInfo()->hp + 2;

			if(playerGetInfo()->hp > playerGetInfo()->maxhp)
				playerGetInfo()->hp = playerGetInfo()->maxhp;
		}
	}
}
