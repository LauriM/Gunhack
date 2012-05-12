#include "globals.h"
#include <stdio.h>
#include "tpl.h"
#include "save.h"
#include "hud.h"

#include "player.h"
#include "map.h"
#include "item.h"
#include "npc.h"

#define LOAD_STATIC_STRUCT(p_struct) fread(&p_struct,sizeof(p_struct),1,input);
#define LOAD_STRUCT(p_struct,p_capacity) fread(&p_struct,(p_capacity)*sizeof(p_struct[0]),1,input);
#define WRITE_STRUCT(p_struct) fwrite(&p_struct, sizeof(p_struct),1,output);

#define REALLOC_ARRAY(p_target,p_capacity) p_target = malloc((p_capacity)*sizeof(*p_target)); 

void saveGame(void){
	FILE* output;
	output = fopen("save","wb");

	save_header_t header;

	header.roomSize         = roomSize;
	header.itemVisSize      = itemVisSize;
	header.itemVisCapacity  = itemVisCapacity;
	header.itemDataSize     = itemDataSize;
	header.itemDataCapacity = itemDataCapacity;
	header.npcDataSize      = npcDataSize;
	header.npcDataCapacity  = npcDataCapacity;

	WRITE_STRUCT(header);
	WRITE_STRUCT(playerInfo);
	WRITE_STRUCT(room);
	WRITE_STRUCT(itemVis);
	WRITE_STRUCT(itemData);
	WRITE_STRUCT(npcData);

	LOG_INFO("Game saved to disk");
	playerGetInfo()->endGame = ENDGAME_REASON_SAVE;

	fclose(output);
}

void loadGame(void){
	FILE* input;

	input = fopen("save","rb");

	save_header_t header;

	LOAD_STATIC_STRUCT(header);
	LOAD_STATIC_STRUCT(playerInfo);

	roomSize         = header.roomSize;
	itemVisSize      = header.itemVisSize;
	itemVisCapacity  = header.itemVisCapacity;
	itemDataSize     = header.itemDataSize;
	itemDataCapacity = header.itemDataCapacity;
	npcDataSize      = header.npcDataSize;
	npcDataCapacity  = header.npcDataCapacity;

	REALLOC_ARRAY(room,roomSize);
	REALLOC_ARRAY(itemVis,itemVisCapacity);
	REALLOC_ARRAY(itemData,itemDataCapacity);
	REALLOC_ARRAY(npcData,npcDataCapacity);

	LOAD_STATIC_STRUCT(room);
	LOAD_STRUCT(itemVis,itemVisSize);
	LOAD_STRUCT(itemData,itemDataSize);
	LOAD_STRUCT(npcData,npcDataSize);

	LOG_INFO("Save loaded from the disk");
}
