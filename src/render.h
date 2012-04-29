#ifndef RENDER_H
#define RENDER_H
#include "globals.h"

extern void renderInit(void);
extern void renderUnInit(void);

extern void printxy(int x,int y,char* ch);
extern void printIntxy(int x,int y,int i);

extern void setColor(color_t colorCode);
extern void setColorOff(color_t colorCode);

#endif
