#include "map.h"
#include "render.h"

void mapInit(void){
	int z,x,y;

	for(z = 0;z < MAP_MAX_DEPTH;z++){
		for(x = 0;x < MAP_MAX_WIDTH;x++){
			for(y = 0;y < MAP_MAX_HEIGHT;y++){
				mapContent[z][x][y] = 0;
			}
		}
	}
}

void mapRender(void){

	int z,x,y;

	for(z = 0;z < MAP_MAX_DEPTH;z++){
		for(x = 0;x < MAP_MAX_WIDTH;x++){
			for(y = 0;y < MAP_MAX_HEIGHT;y++){
				if(mapContent[z][x][y] == 0){
					printxy(x,y,".");
				}else{
					printxy(x,y,"#");
				}
			}
		}
	}
}
