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
