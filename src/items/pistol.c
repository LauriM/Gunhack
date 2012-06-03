#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../input.h"

void itemCall_pistol(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		msgAdd("You reloaded your pistol with 9mm bullets.",TERM_COLOR_GREEN);
	}

	if(action == ITEMACTION_ATTACK){
		//dir_t dir = mapGetDirection();
	}

	return;
}
