#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../input.h"
#include "../map.h"
#include "../npc.h"

void itemCall_law(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		msgAdd("Light AntiTank weapon is a singleshot weapon and cannot be reloaded!",TERM_COLOR_DEFAULT);
		return;
	}

	if(action == ITEMACTION_ATTACK){
		pos_t movement = dirToPos(inputGetDirection());
		pos_t pos      = playerGetInfo()->pos;

		for(int i = 0;i < 100;i++){
			pos.x = pos.x + movement.x;
			pos.y = pos.y + movement.y;

			if(mapGetTileByPos(pos)->block == 1)
				break;
						
			if(npcApplyDamagePos(pos,10) == true)
				break;
		}

		//pos
		int width  = 12;
		int radius = width / 2;

		for(int x = pos.x - radius; x <= (pos.x + radius); ++x){
			for(int y = pos.y - radius; y <= (pos.y + radius); ++y){
				pos_t temp;
				temp.z = pos.z;
				temp.x = x;
				temp.y = y;

				if(temp.x > MAP_MAX_WIDTH -1 || temp.x < 0)
					continue;

				if(temp.y > MAP_MAX_HEIGHT -1 || temp.y < 0)
					continue;

				if(distance(pos,temp) > radius)
					continue;

				if(mapGetTileType(temp) == TILE_ROCK)
					mapEditPoint(temp,TILE_EMPTY);

				mapEditColorPoint(temp,TERM_COLOR_YELLOW);
				npcApplyDamagePos(temp,250);
				mapScanFov();

				//TODO: Self damage for player
				if(playerGetInfo()->pos.x == temp.x && playerGetInfo()->pos.y == temp.y){
					playerGetInfo()->hp -= 150;
				}
			}
		}
		msgAdd("Light antitank weapon is now used!",TERM_COLOR_GREEN);

		//SingleShot so destroy the item!
		itemData[itemId].state = ITEMSTATE_EMPTY;
		itemRemoveSlot(SLOT_WPN);

		return;
	}

	if(action == ITEMACTION_WIELD){//Make sure law always has one bullet in before shooting.
		itemData[itemId].var1 = 1;
		return;
	}
}
