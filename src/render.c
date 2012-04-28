#include "globals.h"
#include "render.h"
#include <ncurses.h>

void renderInit(void){
	initscr();
	if(has_colors() == false){
		endwin();
		printf("Your terminal doesn't support colors...\nFind more modern system/terminal.");
	}

	start_color();

	init_pair(TERM_COLOR_DEFAULT,COLOR_WHITE,COLOR_BLACK);
	init_pair(TERM_COLOR_RED,COLOR_RED,COLOR_BLACK);
	//-snip-
	init_pair(TERM_COLOR_WHITE_RED,COLOR_WHITE,COLOR_RED);
}

void renderUnInit(void){
	clear();
	endwin();
}

void setColor(int colorCode){
	attron(COLOR_PAIR(colorCode));
}

void setColorOff(int colorCode){
	attroff(COLOR_PAIR(colorCode));
}

void printxy(int x,int y,char* ch){
	ASSERT_WIDTH(x);
	ASSERT_HEIGHT(y);

	//+1 to move map down one slot to make room for the prompt
	move(y+1,x);
	printw(ch);
}

void printIntxy(int x,int y,int i){
	ASSERT_WIDTH(x);
	ASSERT_HEIGHT(y);

	//+1 to move map down one slot to make room for the prompt
	move(y+1,x);
	printw("%c",i);
}
