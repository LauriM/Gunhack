#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../input.h"
#include "../map.h"
#include "../npc.h"

void itemCall_pistol(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		msgAdd("You reloaded your pistol with 9mm bullets.",TERM_COLOR_GREEN);
	}

	if(action == ITEMACTION_ATTACK){
		pos_t movement = dirToPos(inputGetDirection());
		pos_t pos = playerGetInfo()->pos;

		//Move one tile at the time

		for(int i = 0;i < 100;i++){
			pos.x = pos.x + movement.x;
			pos.y = pos.y + movement.y;

			printxy(pos.x,pos.y,"|");

			if(mapGetTileByPos(pos)->block == 1){
				mapEditColorPoint(pos,TERM_COLOR_GREY);
				msgAdd("You shot the wall.",TERM_COLOR_DEFAULT);
				return;
			}

			if(npcApplyDamagePos(pos,10) == true){
				msgAdd("You shot an npc.",TERM_COLOR_GREEN);
				return;
			}
		}
	}

	return;
}
