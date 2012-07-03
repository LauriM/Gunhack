#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../input.h"
#include "../map.h"
#include "../npc.h"

void itemCall_shotgun(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		if(itemData[itemId].var1 >= 8){
			msgAdd("Shotgun is full!",TERM_COLOR_DEFAULT);
			return;
		}

		if(playerGetInfo()->ammo_shell <= 0){
			msgAdd("You don't have any ammo for that weapon!",TERM_COLOR_DEFAULT);
			return;
		}

		//Its not full or empty, reload!

		++itemData[itemId].var1;
		--playerGetInfo()->ammo_shell;

		msgAdd("You reloaded one shell to the shotgun.",TERM_COLOR_GREEN);
		return;
	}

	if(action == ITEMACTION_ATTACK){
		if(itemData[itemId].var1 < 1){
			msgAdd("Click.",TERM_COLOR_RED);
			return;
		}

		pos_t movement = dirToPos(inputGetDirection());
		pos_t pos = playerGetInfo()->pos;

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

			int damage = (55 - (distance(playerGetInfo()->pos,pos) * 4));

			if(npcApplyDamagePos(pos,MAX(0,damage)) == true){
				msgAdd("You shot an npc.",TERM_COLOR_GREEN);
				return;
			}
		}
	}
}
