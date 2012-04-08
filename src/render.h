#ifndef RENDER_H
#include <ncurses.h>

extern void renderInit(void);
extern void renderUnInit(void);

extern void printxy(int x,int y,char* ch);

#endif
