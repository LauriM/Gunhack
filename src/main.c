#include <stdio.h>
#include <ncurses.h>

#include "render.h"
#include "player.h"
#include "globals.h"

extern int playerX,playerY;
int key;

//int main(int argc, const char *argv[]){
int main(){
	renderInit();

	#ifdef DEBUG_GETCH_KEY
	while(1){
		key = getch();

		printf("%i",key);
	}
	#endif

	//==========================================================//
	//  Main loop
	//==========================================================//          
	while(1){
		printxy(playerX,playerY,"@");
		key = getch();

		switch(key){
			case 106:  
				movePlayer(DIR_S);
				break;
			case 107:
				movePlayer(DIR_N);
				break;
		}

		clear();
	}

	renderUnInit();
	return 0;
}
