#include "math.h"
#include <stdlib.h>

int random(int max){
	return rand() % max;
}
int randomRange(int min,int max){
	return (rand() % max) + min;
}
