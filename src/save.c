#include <stdio.h>
#include "save.h"
#include "hud.h"

#include "player.h"
#include "map.h"
#include "item.h"

#define WRITE_STRUCT(p_struct) fwrite(&p_struct, sizeof(p_struct),1,output);
#define READ_STRUCT(p_struct)  fread (&p_struct, sizeof(p_struct),1,input);

void saveGame(void){
	FILE* output;

	output = fopen("save","wb");

	WRITE_STRUCT(playerInfo);
	WRITE_STRUCT(room);
	WRITE_STRUCT(tileInfo);
	WRITE_STRUCT(itemVis);
	WRITE_STRUCT(itemData);

	fclose(output);

	LOG_INFO("Game saved to disk");
	hudMessage("Game saved!");
	playerGetInfo()->endGame = 1;
}

void loadGame(void){
	FILE* input;

	input = fopen("save","rb");

	READ_STRUCT(playerInfo);
	READ_STRUCT(room);
	READ_STRUCT(tileInfo);
	READ_STRUCT(itemVis);
	READ_STRUCT(itemData);

	fclose(input);
	
	LOG_INFO("Save loaded from the disk");
}
