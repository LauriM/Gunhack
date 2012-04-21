#include <stdio.h>
#include "save.h"
#include "hud.h"

#include "player.h"
#include "map.h"
#include "item.h"

#define WRITE_STRUCT(p_struct) fwrite(&p_struct, sizeof(&p_struct),1,output);

void saveGame(void){
	FILE* output;

	output = fopen("save","wb");

	WRITE_STRUCT(playerInfo);
	WRITE_STRUCT(room);
	WRITE_STRUCT(tileInfo);
	WRITE_STRUCT(itemVis);
	WRITE_STRUCT(itemData);

	fclose(output);

	hudMessage("Game saved!");
	playerGetInfo()->endGame = 1;
}
