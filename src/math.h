#ifndef MATH_H
#define MATH_H

typedef struct pos_s{
	int x;
	int y;
} pos_t;

/*
typedef struct vec2_s{
	union{
		struct { float x, y; };
		float v[2];
    }
} vec2_t;            
*/

int random(int max);
int randomRange(int min,int max);
int setMinMax(int value,int min,int max);

#endif
