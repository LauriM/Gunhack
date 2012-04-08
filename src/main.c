#include <stdio.h>
#include <ncurses.h>

//int main(int argc, const char *argv[]){
int main(){
	initscr();		   

	printw("hello world!");

	getch();

	endwin();
	return 0;
}
