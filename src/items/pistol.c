#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../input.h"
#include "../map.h"
#include "../npc.h"

void itemCall_pistol(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		if(itemData[itemId].var1 == 12){
			msgAdd("Weapon already loaded!",TERM_COLOR_DEFAULT);
			return;
		}

		if(playerGetInfo()->ammo_9mm == 0){
			msgAdd("You don't have any ammo for that weapon!",TERM_COLOR_DEFAULT);
			return;
		}

		playerGetInfo()->ammo_9mm += itemData[itemId].var1;

		if(playerGetInfo()->ammo_9mm >= 12){
			itemData[itemId].var1      = 12;
			playerGetInfo()->ammo_9mm -= 12;
		}else{
			itemData[itemId].var1     = playerGetInfo()->ammo_9mm;
			playerGetInfo()->ammo_9mm = 0;
		}
		msgAdd("You reloaded your pistol with 9mm bullets.",TERM_COLOR_GREEN);
        return;
	}

	if(action == ITEMACTION_ATTACK){
		if(itemData[itemId].var1 < 1){
            msgAdd("Click.",TERM_COLOR_RED);
			return;
		}

		pos_t movement = dirToPos(inputGetDirection());
		pos_t pos = playerGetInfo()->pos;

		//apply sound
		npcApplyNoiseToPos(pos, 50);

		--itemData[itemId].var1;

		//Move one tile at the time

		for(int i = 0;i < 100;i++){
			pos.x = pos.x + movement.x;
			pos.y = pos.y + movement.y;

			if(mapGetTileByPos(pos)->block == 1){
				mapEditColorPoint(pos,TERM_COLOR_GREY);
				msgAdd("You shot the wall.",TERM_COLOR_DEFAULT);
				return;
			}

			if(npcApplyDamagePos(pos,10) == true)
				return;
		}
	}
}
