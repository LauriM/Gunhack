#include <stdio.h>
#include <ncurses.h>

#include "render.h"

//int main(int argc, const char *argv[]){
int main(){
	renderInit();

	printw("hello world!");

	printxy(20,20,"a");
	printxy(30,30,"@");

	getch();

	renderUnInit();
	return 0;
}
