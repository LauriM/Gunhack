#ifndef SAVE_H
#define SAVE_H

typedef struct{
	//TODO: Add save version
	int roomSize;

	int itemVisSize;

	int itemDataSize;

	int npcDataSize;
} save_header_t;

void saveGame(void);
void loadGame(void);

#endif
