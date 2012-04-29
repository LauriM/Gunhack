#ifndef PLAYER_H

#include "math.h"

typedef enum{
	ENDGAME_REASON_NOTENDED = 0,
	ENDGAME_REASON_SAVE,
	ENDGAME_REASON_GAMEOVER,
	ENDGAME_REASON_WIN
} endgamereason_t;

typedef struct player_s{
	pos_t pos;

	int points;

	int hp;
	int maxhp;

	int exp;
	int level;

	endgamereason_t endGame;
} player_t;

extern struct player_s playerInfo;

extern void playerInit();

extern void playerRender();
extern void playerMove(int direction);
extern void playerInit(void);

extern void playerRandomPosition(void);

extern player_t* playerGetInfo(void);

#endif
