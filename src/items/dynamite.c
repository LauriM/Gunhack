#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../map.h"
#include "../npc.h"

void itemCall_dynamite(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		msgAdd("Dynamite is on fire! You drop it to floor.",TERM_COLOR_GREEN);

		itemData[itemId].var1  = 10;
		itemData[itemId].state = ITEMSTATE_GROUND; //Drop it
		itemData[itemId].pos   = playerGetInfo()->pos;
	}

	if(action == ITEMACTION_TICK){
		if(itemData[itemId].var1 > 0){
			--itemData[itemId].var1;

			if(itemData[itemId].var1 == 1 || itemData[itemId].var1 < 0){
				int width  = 7;
				int radius = width / 2;

				for(int x = itemData[itemId].pos.x - radius; x <= (itemData[itemId].pos.x + radius); ++x){
					for(int y = itemData[itemId].pos.y - radius; y <= (itemData[itemId].pos.y + radius); ++y){
						pos_t temp;
						temp.z = itemData[itemId].pos.z;
						temp.x = x;
						temp.y = y;

						if(temp.x > MAP_MAX_WIDTH -1 || temp.x < 0)
							continue;

						if(temp.y > MAP_MAX_HEIGHT -1 || temp.y < 0)
							continue;

						if(distance(itemData[itemId].pos,temp) > radius)
							continue;

						if(mapGetTileType(temp) == TILE_ROCK)
							mapEditPoint(temp,TILE_EMPTY);

						mapEditColorPoint(temp,TERM_COLOR_YELLOW);
						npcApplyDamagePos(temp,250);
						mapScanFov();

						if(playerGetInfo()->pos.x == temp.x && playerGetInfo()->pos.y == temp.y){
							playerGetInfo()->hp -= 150;
						}
					}
				}

				msgAdd("Dynamite explodes!",TERM_COLOR_RED);
				itemData[itemId].state = ITEMSTATE_EMPTY;
			}

		}
	}

	return;
} 
