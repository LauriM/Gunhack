#ifndef MATH_H
#define MATH_H

#define MAX(p_v1, p_v2) ((p_v1) > (p_v2) ? (p_v1) : (p_v2))
#define MIN(p_v1, p_v2) ((p_v1) < (p_v2) ? (p_v1) : (p_v2))
#define CAP(p_value, p_min, p_max) (p_value = MAX(p_min, MIN(p_max, p_value)))

typedef struct pos_s{
	int x;
	int y;
	int z;
} pos_t;

/*
typedef struct vec2_s{
	union{
		struct { float x, y; };
		float v[2];
    }
} vec2_t;            
*/

void mathInit();

int randomMax(int max);
int randomRange(int min,int max);
double distance(pos_t pos1,pos_t pos2);
#endif
