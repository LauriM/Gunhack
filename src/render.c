#include "render.h"
#include <ncurses.h>

void renderInit(void){
	initscr();
}

void renderUnInit(void){
	clear();
	endwin();
}

void printxy(int x,int y,char* ch){
	move(y,x);
	printw(ch);
}

void printIntxy(int x,int y,int i){
	move(y,x);
	printw("%c",i);
}
