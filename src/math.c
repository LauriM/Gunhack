#include <math.h>
#include "math.h"
#include <stdlib.h>
#include <time.h>

void mathInit(){
	srand(time(NULL));
}

int randomMax(int max){
	return rand() % (max+1);
}

int randomRange(int min,int max){
	return (rand() % (max-min+1)) + min;
}

double distance(pos_t pos1,pos_t pos2){
	double dx = pos1.x - pos2.x;
	double dy = pos1.y - pos2.y;

	return sqrt(dx*dx + dy*dy);
}

pos_t dirToPos(dir_t dir){
	pos_t pos;
	pos.z = 0;
	pos.x = 0;
	pos.y = 0;

	switch(dir){
		case DIR_N:
			pos.y = -1;
			break;
		case DIR_S:
			pos.y = 1;
			break;
		case DIR_E:
			pos.x = 1;
			break;
		case DIR_W:
			pos.x = -1;
			break;
		case DIR_SE:
			pos.y = 1;
			pos.x = 1;
			break;
		case DIR_NW:
			pos.y = -1;
			pos.x = -1;
			break;
		case DIR_SW:
			pos.y = 1;
			pos.x = -1;
			break;
		case DIR_NE:
			pos.y = -1;
			pos.x = 1;
			break;
		case DIR_COUNT:
			LOG_ERROR("This should never happen!");
			break;
	}

	return pos;
}
