#include "globals.h"
#include <stdio.h>
#include "save.h"
#include "hud.h"

#include "player.h"
#include "map.h"
#include "item.h"
#include "npc.h"

void saveGame(void){
	FILE* file = fopen("save","wb");

	save_header_t header;

	header.roomSize = roomSize;
	header.itemVisSize = itemVisSize;
	header.itemDataSize = itemDataSize;
	header.npcDataSize = npcDataSize;

	fwrite(&header, sizeof(header), 1, file);
	fwrite(&playerInfo, sizeof(playerInfo), 1, file);
	fwrite(room, sizeof(room[0]), roomSize, file);
	fwrite(itemVis, sizeof(itemVis[0]), itemVisSize, file);
	fwrite(itemData, sizeof(itemData[0]), itemDataSize, file);
	fwrite(npcData, sizeof(npcData[0]), npcDataSize, file);

	LOG_INFO("Game saved to disk");
	playerGetInfo()->endGame = ENDGAME_REASON_SAVE;

	fclose(file);
}

void loadGame(void){
	FILE* file = fopen("save","rb");

	save_header_t header;

	fread(&header, sizeof(header), 1, file);
	fread(&playerInfo, sizeof(playerInfo), 1, file);

	roomSize = header.roomSize;
	itemVisCapacity = itemVisSize = header.itemVisSize;
	itemDataCapacity = itemDataSize = header.itemDataSize;
	npcDataCapacity = npcDataSize = header.npcDataSize;

	room = realloc(room,roomSize * sizeof(room[0]));
	itemVis = realloc(itemVis,itemVisCapacity * sizeof(itemVis[0]));
	itemData = realloc(itemData,itemDataCapacity * sizeof(itemVis[0]));
	npcData = realloc(npcData,npcDataCapacity * sizeof(itemVis[0]));

	fread(room, sizeof(room[0]), roomSize, file);
	fread(itemVis, sizeof(itemVis[0]), itemVisSize, file);
	fread(itemData, sizeof(itemData[0]), itemDataSize, file);
	fread(npcData, sizeof(npcData[0]), npcDataSize, file);

	LOG_INFO("Save loaded from the disk");

	fclose(file);
}
