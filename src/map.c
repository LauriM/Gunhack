#include "globals.h"
#include "map.h"
#include "render.h"
#include "player.h"
#include "math.h"

int currentRoom;
struct tile_s tileInfo[TILE_COUNT];
int world[WORLD_SIZE_Z][WORLD_SIZE_X][WORLD_SIZE_Y]; //Contains RoomId
struct room_s room[WORLD_ROOM_COUNT]; //Room list

void tileInit(void){
	tileInfo[TILE_EMPTY].symbol    = ' ';
	tileInfo[TILE_EMPTY].visBlock  = 0;
	tileInfo[TILE_EMPTY].moveBlock = 0;

	tileInfo[TILE_ROCK].symbol    = '#';
	tileInfo[TILE_ROCK].visBlock  = 1;
	tileInfo[TILE_ROCK].moveBlock = 1;
}

/**
 * Setup the world array with 0's
 * Some functionality may be added
 */
void worldInit(void){
	int z,x,y;
	int i = 0;

	for(z = 0;z < WORLD_SIZE_Z;z++){
		for(x = 0;x < WORLD_SIZE_X;x++){
			for(y = 0;y < WORLD_SIZE_Y;y++){
				world[z][x][y] = i;
				i++;
			}
		}
	}
}

void roomInit(int id){
	int x,y;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			room[id].mapData[x][y] = 0;
		}
	}
}

void mapRender(void){
	int x,y;

	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			setColor(room[currentRoom].colorData[x][y]);
			printIntxy(x,y,mapGetTileByPos(currentRoom,x,y).symbol);
		}
	}
}

void mapCreateRoom(int id){
	int x,y;

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

	roomX = playerX - 1;
	roomY = playerY - 1; 

	mapEditBox(id,roomX,roomY,3,3,0);

	//Create the rooms
	int roomCount = (random(GEN_ROOM_MAX_COUNT)+GEN_ROOM_MIN_COUNT);

	int triesCount = 0;
	int i = 0;
	while(i < roomCount){
		roomWidth = (random(8) + 3);//TODO: change 5 to max size
		roomHeight = (random(8) + 3);
		roomX = random(MAP_MAX_WIDTH-roomWidth-1);
		roomY = random(MAP_MAX_HEIGHT-roomHeight-1);

		//Check if the box exists (+- are to make sure rooms wont hit)
		if(mapCheckTileCoords(id,roomX-1,roomY-1,roomWidth+2,roomHeight+2,0) == 1){
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

	int tunnelCount = roomCount + random(GEN_TUNNEL_EXTRA);
	i = 0;
	digX = playerX;
	digY = playerY;

	while(i < tunnelCount){
		digLeft       = random(GEN_TUNNEL_MAX_LENGTH);
		digLeftRotate = random(GEN_TUNNEL_MAX_ROTATE_STEP);
		digDir        = random(3);
		setMinMax(digLeft,GEN_TUNNEL_MIN_ROTATE,GEN_TUNNEL_MAX_ROTATE);

		while(digLeft > 0){
			mapEditPoint(id,digX,digY,TILE_EMPTY);
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
				digDir = random(3);
			}
		}
		i++;//One tunnel done
	}

	//Finally, setup the "borders" of the map as solid rock
	//TODO: Consider more world after the sides(?)
	//TODO: Consider indestructible walls at borders(?)
	mapEditBox(id , 0               , 0              , MAP_MAX_WIDTH , 1              , TILE_ROCK);//Top
	mapEditBox(id , 0               , MAP_MAX_HEIGHT-1 , MAP_MAX_WIDTH , 1              , TILE_ROCK);//bottom
	mapEditBox(id , 0               , 0              , 1             , MAP_MAX_HEIGHT , TILE_ROCK); //Left
	mapEditBox(id , MAP_MAX_WIDTH-1 , 0              , 1             , MAP_MAX_HEIGHT , TILE_ROCK); //Right

	//Now that the room is generated, lets setup the color layer
	for(x = 0;x < MAP_MAX_WIDTH;x++){
		for(y = 0;y < MAP_MAX_HEIGHT;y++){
			room[id].colorData[x][y] = TERM_COLOR_DEFAULT;
		}
	}
}

void mapEditPoint(int id,int x,int y,int tileType){
	assert(tileType >= 0);
	assert(tileType < TILE_COUNT);

	room[id].mapData[x][y] = tileType;
}

void mapEditBox(int id,int boxX,int boxY,int width,int height,int tileType){

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
	for(int y = boxY;y < boxY + height;y++){
		for(int x = boxX;x < boxX + width;x++){
			if(room[id].mapData[x][y] == tileType){
				return 0;
			}
		}
	}
	return 1;
}

struct tile_s mapGetTileByPos(int id,int x,int y){
	assert(x < MAP_MAX_WIDTH);
	assert(y < MAP_MAX_HEIGHT);
	assert(id >= 0);
	assert(id < WORLD_ROOM_COUNT);

	return tileInfo[room[id].mapData[x][y]];
}
