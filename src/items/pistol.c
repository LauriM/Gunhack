#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../input.h"
#include "../map.h"
#include "../npc.h"

void itemCall_pistol(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		if(playerGetInfo()->wpnAmmo == 1){
			msgAdd("Weapon already loaded!",TERM_COLOR_DEFAULT);
			return;
		}

		if(playerGetInfo()->ammo_9mm == 0){
			msgAdd("You don't have any ammo for that weapon!",TERM_COLOR_DEFAULT);
			return;
		}

		playerGetInfo()->ammo_9mm += playerGetInfo()->wpnAmmo;

		if(playerGetInfo()->ammo_9mm >= 12){
			playerGetInfo()->wpnAmmo   = 12;
			playerGetInfo()->ammo_9mm -= 12;
		}else{
			playerGetInfo()->wpnAmmo  = playerGetInfo()->ammo_9mm;
			playerGetInfo()->ammo_9mm = 0;
		}
		msgAdd("You reloaded your pistol with 9mm bullets.",TERM_COLOR_GREEN);
        return;
	}

	if(action == ITEMACTION_ATTACK){
		if(playerGetInfo()->wpnAmmo < 1){
            msgAdd("Click.",TERM_COLOR_RED);
			return;
		}

		pos_t movement = dirToPos(inputGetDirection());
		pos_t pos = playerGetInfo()->pos;

		--playerGetInfo()->wpnAmmo;

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
}
