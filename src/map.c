#include "globals.h"
#include "map.h"
#include "render.h"
#include "player.h"
#include <stdlib.h>
#include <math.h>
#include "math.h"
#include "item.h"

tile_t tileInfo[TILE_COUNT];
room_t room[WORLD_ROOM_COUNT]; //Room list

void tileInit(void){
	tileInfo[TILE_VOID].symbol        = ' ';
	tileInfo[TILE_VOID].block         = 0;

	tileInfo[TILE_EMPTY].symbol       = '.';
	tileInfo[TILE_EMPTY].block        = 0;

	tileInfo[TILE_INDESTRUCTIBLE_ROCK].symbol        = '#';
	tileInfo[TILE_INDESTRUCTIBLE_ROCK].block         = 1;

	tileInfo[TILE_ROCK].symbol        = '#';
	tileInfo[TILE_ROCK].block         = 1;

	tileInfo[TILE_STAIRS_UP].symbol   = '<';
	tileInfo[TILE_STAIRS_UP].block    = 0;

	tileInfo[TILE_STAIRS_DOWN].symbol = '>';
	tileInfo[TILE_STAIRS_DOWN].block  = 0;
}

void worldInit(void){
	for(int i = 0;i < WORLD_ROOM_COUNT;i++){
		room[i].roomType = ROOM_TYPE_UNINITIALIZED;
	}
}

void roomInit(int id){
	int x,y;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			room[id].mapData[x][y]   = TILE_VOID;
			room[id].visData[x][y]   = TILE_VOID;
			room[id].colorData[x][y] = TERM_COLOR_DEFAULT;
		}
	}
}

void mapRender(void){
	int x,y;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			setColor(room[playerGetInfo()->playerZ].colorData[x][y]);
			printIntxy(x,y,mapGetVisByPos(playerGetInfo()->playerZ,x,y)->symbol);
			setColorOff(room[playerGetInfo()->playerZ].colorData[x][y]);
		}
	}
}

void mapCreateRoom(int id){
	//TODO: Clean up this function bit, its kinda a mess
	int x,y;

	room[id].roomType = ROOM_TYPE_DUNGEON;

	//Clear the room
	roomInit(id);
	
	//First, lets add the concrete
	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			room[id].mapData[x][y] = TILE_ROCK;
		}
	}

	int roomX      = 0;
	int roomY      = 0;
	int roomWidth  = 0;
	int roomHeight = 0;

	roomX = playerGetInfo()->playerX - 1;
	roomY = playerGetInfo()->playerY - 1; 

	mapEditBox(id,roomX,roomY,3,3,TILE_EMPTY);

	//Create the rooms
	int roomCount = (randomMax(GEN_ROOM_MAX_COUNT)+GEN_ROOM_MIN_COUNT);

	int triesCount = 0;
	int i = 0;
	while(i < roomCount){
		roomWidth = (randomMax(8) + 3);//TODO: change 5 to max size
		roomHeight = (randomMax(8) + 3);   //TODO: Move these to globals
		roomX = randomMax(MAP_MAX_WIDTH-roomWidth-1);
		roomY = randomMax(MAP_MAX_HEIGHT-roomHeight-1);

		//Check if the box exists (+- are to make sure rooms wont hit)
		if(mapCheckTileCoords(id,roomX,roomY,roomWidth+2,roomHeight+2,0) == 1){
			mapEditBox(id,roomX,roomY,roomWidth,roomHeight,TILE_EMPTY);
			i++;
		}else{
			triesCount++;
			if(triesCount > GEN_MAX_TRIES){
				//hit the generator max size count, won't create the room
				i++;
			}
		}
	}

	//Create the tunnels between the rooms
	int digX;
	int digY;
	int digDir;
	int digLeft;
	int digLeftRotate;

	int tunnelCount = roomCount + (randomMax(GEN_TUNNEL_COUNT_MAX - GEN_TUNNEL_COUNT_MIN) + GEN_TUNNEL_COUNT_MIN);
	i = 0;
	digX = playerGetInfo()->playerX;
	digY = playerGetInfo()->playerY;

	while(i < tunnelCount){
		digLeft       = randomMax(GEN_TUNNEL_MAX_LENGTH);
		digLeftRotate = randomMax(GEN_TUNNEL_MAX_ROTATE_STEP);
		digDir        = randomMax(3);

		while(digLeft > 0){
			if(digX >= 0 && digX < MAP_MAX_WIDTH && digY >= 0 && digY < MAP_MAX_HEIGHT){
				mapEditPoint(id,digX,digY,TILE_EMPTY);
			}else{
				digLeft       = 0;
				digLeftRotate = 0;
			}
			digLeft--;
			digLeftRotate--;

			switch(digDir){
				case DIR_N:
					digY = digY - 1;
					break;
				case DIR_S:
					digY = digY + 1;
					break;
				case DIR_W:
					digX = digX - 1;
					break;
				case DIR_E:
					digX = digX + 1;
					break;
			}

			if(digLeftRotate < 0){
				digDir = randomMax(3);
			}
		}
		i++;//One tunnel done
	}

	//Finally, setup the "borders" of the map as solid rock
	mapEditBox(id , 0               , 0              , MAP_MAX_WIDTH-1 , 1              , TILE_INDESTRUCTIBLE_ROCK);//Top
	mapEditBox(id , 0               , MAP_MAX_HEIGHT-1 , MAP_MAX_WIDTH-1 , 1              , TILE_INDESTRUCTIBLE_ROCK);//bottom
	mapEditBox(id , 0               , 0              , 1             , MAP_MAX_HEIGHT-1 , TILE_INDESTRUCTIBLE_ROCK); //Left
	mapEditBox(id , MAP_MAX_WIDTH-1 , 0              , 1             , MAP_MAX_HEIGHT-1 , TILE_INDESTRUCTIBLE_ROCK); //Right

	//Now that the room is generated, lets setup the color layer
	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			room[id].colorData[x][y] = TERM_COLOR_DEFAULT;
		}
	}

	//Lets add some dungeon features!
	
	//First up and down stairs
	i = 0;
	while(i < 1){
		x = randomMax(MAP_MAX_WIDTH);
		y = randomMax(MAP_MAX_HEIGHT);
		if(room[id].mapData[x][y] == TILE_EMPTY){
			room[id].mapData[x][y] = TILE_STAIRS_UP;
			i++;//Break out
		}
	}

	i = 0;
	while(i < 1){
		x = randomMax(MAP_MAX_WIDTH);
		y = randomMax(MAP_MAX_HEIGHT);
		if(room[id].mapData[x][y] == TILE_EMPTY){
			room[id].mapData[x][y] = TILE_STAIRS_DOWN;
			i++;//Break out
		}
	}

	itemSpawnRandom(id);

	LOG_INFO("Level generated");
}

void mapEditPoint(int id,int x,int y,int tileType){
	assert(tileType >= 0);
	assert(tileType < TILE_COUNT);
	assert(x >= 0);
	assert(x < MAP_MAX_WIDTH);
	assert(y >= 0);
	assert(y < MAP_MAX_HEIGHT);

	room[id].mapData[x][y] = tileType;
}

void mapEditBox(int id,int boxX,int boxY,int width,int height,int tileType){
	assert(tileType >= 0);
	assert(tileType < TILE_COUNT);
	assert(boxX >= 0);
	assert(boxX < MAP_MAX_WIDTH);
	assert(boxY >= 0);
	assert(boxY < MAP_MAX_HEIGHT);
    assert((boxX + width) >= 0);
	assert((boxX + width) < MAP_MAX_WIDTH + 1);//+1 because width starts from 1, not from 0. Causing issues on this calculation
	assert((boxY + height) >= 0);
	assert((boxY + height) < MAP_MAX_HEIGHT + 1);

	for(int y = boxY;y < boxY + height;y++){
		for(int x = boxX;x < boxX + width;x++){
			room[id].mapData[x][y] = tileType;
		}
	}
}

/**
 * Returns count of certain tileType in the area
 * Used to check if area is clear
 */
int mapSearchTileCoords(int id,int boxX,int boxY,int width,int height,int tileType){
	assert(tileType >= 0);
	assert(tileType < TILE_COUNT);
	assert(boxX >= 0);
	assert(boxX < MAP_MAX_WIDTH);
	assert(boxY >= 0);
	assert(boxY < MAP_MAX_HEIGHT);
	assert((boxX + width) >= 0);
	assert((boxX + width) < MAP_MAX_WIDTH + 1);//+1 because width starts from 1, not from 0. Causing issues on this calculation
	assert((boxY + height) >= 0);
	assert((boxY + height) < MAP_MAX_HEIGHT + 1);

	int count = 0;
	for(int y = boxY;y < boxY + height;y++){
		for(int x = boxX;x < boxX + width;x++){
			if(room[id].mapData[x][y] == tileType){
				count++;
			}
		}
	}
	return count;
}

/**
 * Check area that it only contains given tile type
 * Returns bool. 1 when ok, 0 when not true
 */
int mapCheckTileCoords(int id,int boxX,int boxY,int width,int height,int tileType){
	assert(tileType >= 0);
	assert(tileType < TILE_COUNT);
	assert(boxX >= 0);
	assert(boxX < MAP_MAX_WIDTH);
	assert(boxY >= 0);
	assert(boxY < MAP_MAX_HEIGHT);
	assert((boxX + width) >= 0);
	assert((boxX + width) < MAP_MAX_WIDTH + 1);//+1 because width starts from 1, not from 0. Causing issues on this calculation
	assert((boxY + height) >= 0);
	assert((boxY + height) < MAP_MAX_HEIGHT + 1);

	for(int y = boxY;y < boxY + height;y++){
		for(int x = boxX;x < boxX + width;x++){
			if(room[id].mapData[x][y] == tileType){
				return 0;
			}
		}
	}
	return 1;
}

struct tile_s* mapGetTileByPos(int z,int x,int y){
	assert(x < MAP_MAX_WIDTH);
	assert(x >= 0);
	assert(y < MAP_MAX_HEIGHT);
	assert(y >= 0);
	assert(z >= 0);
	assert(z < WORLD_ROOM_COUNT);

	return &tileInfo[room[z].mapData[x][y]];
}

struct tile_s* mapGetVisByPos(int z,int x,int y){
	assert(x < MAP_MAX_WIDTH);
	assert(x >= 0);
	assert(y < MAP_MAX_HEIGHT);
	assert(y >= 0);
	assert(z >= 0);
	assert(z < WORLD_ROOM_COUNT);

	return &tileInfo[room[z].visData[x][y]];
}

int mapLosCheck(int x1, int y1, int x2, int y2) {
	assert(x1 < MAP_MAX_WIDTH);
	assert(x1 >= 0);
	assert(y1 < MAP_MAX_HEIGHT);
	assert(y1 >= 0);
	assert(x2 < MAP_MAX_WIDTH);
	assert(x2 >= 0);
	assert(y2 < MAP_MAX_HEIGHT);
	assert(y2 >= 0);

	float x_mul;
	float y_mul;
	int len;
	int dx = x2 - x1;
	int dy = y2 - y1;

	if(x1 == x2 && y1 == y2)
	return true;

	// Check which delta axis is longer
	if(abs(dx) > abs(dy)){
		x_mul = 1.0f;
		y_mul = dy/((float)dx);
		len = dx;
	}else{
		x_mul = dx/((float)dy);
		y_mul = 1.0f;
		len = dy;
	}

	// Raycast
	for(int i=0; i != len; len < 0 ? --i : ++i){

		//int x = x1 + ceil(i*x_mul);
		//int y = y1 + ceil(i*y_mul);

		int x = x1 + ((dx < 0) ? ceil(i*x_mul) : i*x_mul);
		int y = y1 + ((dy < 0) ? ceil(i*y_mul) : i*y_mul);


		if(mapGetTileByPos(playerGetInfo()->playerZ, x, y)->block == 1){
			return false;
		}
	}

	// Nothing was hit in the raycast
	return true;
}

void mapScanFov(void){
	int x,y;
	assert(mapGetTileByPos(playerGetInfo()->playerZ, playerGetInfo()->playerX, playerGetInfo()->playerY)->block != 1);//Player should not be in the visblock thing

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			if(mapLosCheck(playerGetInfo()->playerX,playerGetInfo()->playerY,x,y) == 1){
				//Player can see, lets move the tile to the visualData table
				room[playerGetInfo()->playerZ].visData[x][y] = room[playerGetInfo()->playerZ].mapData[x][y];
			}
		}
	}
}

void mapCheatSeeAll(void){
	int x,y;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			room[playerGetInfo()->playerZ].visData[x][y] = room[playerGetInfo()->playerZ].mapData[x][y];
		}
	}
}

pos_t mapFindTilePos(int roomId,tiletype_t tileType){
	int x,y;

	pos_t pos;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			if(room[roomId].mapData[x][y] == tileType){
				pos.x = x;
				pos.y = y;
				return pos;
			}
		}
	}

	pos.x = 0;
	pos.x = 0;

	return pos;
}
