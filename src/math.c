#include "math.h"
#include <stdlib.h>

int random(int max){
	return rand() % max;
}

int setMinMax(int value,int min,int max){
	if(value > max){
		value = max;
		return value;
	}

	if(value < min){
		value = min;
		return value;
	}

	return value;
}
