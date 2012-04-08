#include <stdio.h>
#include <ncurses.h>

#include "render.h"
#include "player.h"
#include "globals.h"
#include "input.h"

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

		inputHandle();

		clear();
	}

	renderUnInit();
	return 0;
}
