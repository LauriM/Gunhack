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
