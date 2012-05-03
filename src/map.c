#include "globals.h"
#include "map.h"
#include "render.h"
#include "player.h"
#include <stdlib.h>
#include <math.h>
#include "math.h"
#include "item.h"
#include "npc.h"

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
			setColor(room[playerGetInfo()->pos.z].colorData[x][y]);

			pos_t pos;
			pos.z = playerGetInfo()->pos.z;
			pos.x = x;
			pos.y = y;

			printIntxy(x,y,mapGetVisByPos(pos)->symbol);
			setColorOff(room[playerGetInfo()->pos.z].colorData[x][y]);
		}
	}
}

void mapCreateRoom(int id){//TODO: Clean up this function bit, its kinda a mess
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

	roomX = playerGetInfo()->pos.x- 1;
	roomY = playerGetInfo()->pos.y- 1; 

	mapEditBox(id,roomX,roomY,3,3,TILE_EMPTY);

	//Create the rooms
	int roomCount = (randomMax(GEN_ROOM_MAX_COUNT)+GEN_ROOM_MIN_COUNT);

	int triesCount = 0;
	int i = 0;
	while(i < roomCount){
		roomWidth = (randomMax(8) + 3);
		roomHeight = (randomMax(8) + 3);   //TODO: Move these to globals
		roomX = randomMax(MAP_MAX_WIDTH-roomWidth-2);
		roomY = randomMax(MAP_MAX_HEIGHT-roomHeight-2);

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
	digX = playerGetInfo()->pos.x;
	digY = playerGetInfo()->pos.y;

	while(i < tunnelCount){
		digLeft       = randomMax(GEN_TUNNEL_MAX_LENGTH);
		digLeftRotate = randomMax(GEN_TUNNEL_MAX_ROTATE_STEP);
		digDir        = randomMax(3);
		CAP(digLeftRotate,GEN_TUNNEL_MIN_ROTATE,GEN_TUNNEL_MAX_ROTATE);

		while(digLeft > 0){
			if(digX >= 0 && digX < MAP_MAX_WIDTH && digY >= 0 && digY < MAP_MAX_HEIGHT){
				pos_t pos;
				pos.z = id;
				pos.x = digX;
				pos.y = digY;

				mapEditPoint(pos,TILE_EMPTY);
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
	npcSpawnRandom(id);

	LOG_INFO("Level generated");
}

void mapEditPoint(pos_t pos,tiletype_t tileType){
	ASSERT_TILE_TYPE(tileType);
	ASSERT_POS_T(pos);

	room[pos.z].mapData[pos.x][pos.y] = tileType;
}

void mapEditBox(int id,int boxX,int boxY,int width,int height,int tileType){
	ASSERT_TILE_TYPE(tileType);
	ASSERT_WIDTH(boxX);
	ASSERT_HEIGHT(boxY);
	ASSERT_ROOM(id);
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
	ASSERT_TILE_TYPE(tileType);
	ASSERT_WIDTH(boxX);
	ASSERT_HEIGHT(boxY);
	ASSERT_ROOM(id);
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
	ASSERT_TILE_TYPE(tileType);
	ASSERT_WIDTH(boxX);
	ASSERT_HEIGHT(boxY);
	ASSERT_ROOM(id);
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

struct tile_s* mapGetTileByPos(pos_t pos){
	ASSERT_ROOM(pos.z);
	ASSERT_WIDTH(pos.x);
	ASSERT_HEIGHT(pos.y);

	return &tileInfo[room[pos.z].mapData[pos.x][pos.y]];
}

struct tile_s* mapGetVisByPos(pos_t pos){
	ASSERT_ROOM(pos.z);
	ASSERT_WIDTH(pos.x);
	ASSERT_HEIGHT(pos.y);

	return &tileInfo[room[pos.z].visData[pos.x][pos.y]];
}

int mapLosCheck(int x1, int y1, int x2, int y2) {
	ASSERT_WIDTH(x1);
	ASSERT_HEIGHT(y1);
	ASSERT_WIDTH(x2);
	ASSERT_HEIGHT(y2);

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

		pos_t pos;
		pos.z = playerGetInfo()->pos.z;
		pos.x = x;
		pos.y = y;

		if(mapGetTileByPos(pos)->block == 1){
			return false;
		}
	}

	// Nothing was hit in the raycast
	return true;
}

void mapScanFov(void){
	int x,y;
	assert(mapGetTileByPos(playerGetInfo()->pos)->block != 1);//Player should not be in the visblock thing

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			if(mapLosCheck(playerGetInfo()->pos.x,playerGetInfo()->pos.y,x,y) == 1){
				//Player can see, lets move the tile to the visualData table
				room[playerGetInfo()->pos.z].visData[x][y] = room[playerGetInfo()->pos.z].mapData[x][y];
			}
		}
	}
}

void mapCheatSeeAll(void){
	int x,y;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			room[playerGetInfo()->pos.z].visData[x][y] = room[playerGetInfo()->pos.z].mapData[x][y];
		}
	}
}

pos_t mapFindTilePos(int roomId,tiletype_t tileType){
	ASSERT_TILE_TYPE(tileType);
	ASSERT_ROOM(roomId);

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


void mapEditColorPoint(pos_t pos,color_t color){
	room[pos.z].colorData[pos.x][pos.y] = color;
}
