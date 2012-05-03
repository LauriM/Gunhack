#include "math.h"
#include <stdlib.h>

int randomMax(int max){
	return rand() % (max+1);
}

int randomRange(int min,int max){
	return (rand() % (max-min+1)) + min;
}
