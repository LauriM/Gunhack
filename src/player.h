#ifndef PLAYER_H

struct player_s{
	int playerX;
	int playerY;
	int endGame;

	//TODO: move currentRoom here to make saving games possible!
};

extern void playerInit();

extern void playerRender();
extern void playerMove(int direction);
extern void playerInit(void);

extern void playerRandomPosition(void);

extern struct player_s* playerGetInfo(void);

#endif
