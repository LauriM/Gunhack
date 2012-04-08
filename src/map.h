#ifndef MAP_H
#define MAP_H

#include "globals.h"

int mapContent[MAP_MAX_DEPTH][MAP_MAX_WIDTH][MAP_MAX_HEIGHT];

void mapInit(void);
void mapRender(void);

#endif
