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
	int hostile_kills;
	int innocent_kills;

	int hp;
	int maxhp;

	int exp;
	int level;

	int turn;

	int ammo_9mm;
	int ammo_39mm;
	int ammo_shell;
	int ammo_rockets;

	endgamereason_t endGame;
} player_t;

struct player_s playerInfo;

void playerInit();

void playerRender();
void playerMove(int direction);
void playerInit(void);

void playerRandomPosition(void);

player_t* playerGetInfo(void);

void playerIncTurn(void);

void playerAddPoints(int amount);
void playerAddExp(int amount);
void playerAddKill(int relation);

/*
 * hurts the player
 * Returns: amount of damage done
 */
int playerApplyDmg(int amount);

void playerDumpToFile();

#endif
