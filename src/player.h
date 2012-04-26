#ifndef PLAYER_H

typedef struct player_s{
	int playerX;
	int playerY;
	int playerZ;

	int points;

	int hp;
	int maxhp;

	int exp;
	int level;

	int endGame;
} player_t;

extern struct player_s playerInfo;

extern void playerInit();

extern void playerRender();
extern void playerMove(int direction);
extern void playerInit(void);

extern void playerRandomPosition(void);

extern player_t* playerGetInfo(void);

#endif
