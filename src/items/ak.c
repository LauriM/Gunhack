
#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../input.h"
#include "../map.h"
#include "../npc.h"

void itemCall_ak47(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		if(playerGetInfo()->wpnAmmo == 1){
			msgAdd("Weapon already loaded!",TERM_COLOR_DEFAULT);
			return;
		}

		playerGetInfo()->ammo_39mm += playerGetInfo()->wpnAmmo;

		if(playerGetInfo()->ammo_39mm >= 30){
			playerGetInfo()->wpnAmmo   = 30;
			playerGetInfo()->ammo_39mm -= 30;
		}else{
			playerGetInfo()->wpnAmmo  = playerGetInfo()->ammo_39mm;
			playerGetInfo()->ammo_39mm = 0;
		}
		msgAdd("You reloaded your ak with 39mm bullets.",TERM_COLOR_GREEN);
		return;
	}

	if(action == ITEMACTION_ATTACK){
		int shots = 3;

		pos_t movement = dirToPos(inputGetDirection());

start:
		while(shots > 0){
			--shots;
			if(playerGetInfo()->wpnAmmo < 1){
				msgAdd("Click.",TERM_COLOR_RED);
				continue;
			}

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
					goto start;
				}

				if(npcApplyDamagePos(pos,10) == true){
					msgAdd("You shot an npc.",TERM_COLOR_GREEN);
					goto start;
				}
			}
		}
	}
}
