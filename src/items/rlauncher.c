#include "../globals.h"
#include "../item.h"
#include "../msg.h"
#include "../player.h"
#include "../input.h"
#include "../map.h"
#include "../npc.h"

void itemCall_rLauncher(int itemId,itemaction_t action){
	if(action == ITEMACTION_USE){
		if(playerGetInfo()->wpnAmmo == 1){
			msgAdd("Weapon already loaded!",TERM_COLOR_DEFAULT);
			return;
		}

		playerGetInfo()->ammo_rockets += playerGetInfo()->wpnAmmo;

		if(playerGetInfo()->ammo_rockets>= 1){
			playerGetInfo()->wpnAmmo   = 1;
			playerGetInfo()->ammo_rockets -= 1;
		}else{
			playerGetInfo()->wpnAmmo  = playerGetInfo()->ammo_rockets;
			playerGetInfo()->ammo_rockets = 0;
		}
		msgAdd("You reloaded your rocket launcher.",TERM_COLOR_GREEN);
        return;
	}

	if(action == ITEMACTION_ATTACK){
		if(playerGetInfo()->wpnAmmo < 1){
            msgAdd("Click.",TERM_COLOR_RED);
			return;
		}

		pos_t movement = dirToPos(inputGetDirection());
		pos_t pos      = playerGetInfo()->pos;

		--playerGetInfo()->wpnAmmo;

		//Move one tile at the time

		for(int i = 0;i < 100;i++){
			pos.x = pos.x + movement.x;
			pos.y = pos.y + movement.y;

			if(mapGetTileByPos(pos)->block == 1)
				break;

			if(npcApplyDamagePos(pos,10) == true)
				break;
		}

		//pos
		int width  = 6;
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
				npcApplyDamagePos(temp,50);
				mapScanFov();

				//TODO: Self damage for player
				if(playerGetInfo()->pos.x == temp.x && playerGetInfo()->pos.y == temp.y){
					playerGetInfo()->hp -= 50;
				}
			}
		}
	}
}
