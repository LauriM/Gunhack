#ifndef PLAYER_H

int playerX;
int playerY;

extern void playerRender();
extern void playerMove(int direction);
extern void playerInit(void);

extern void playerRandomPosition(void);

#endif
