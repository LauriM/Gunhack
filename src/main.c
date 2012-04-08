#include <stdio.h>
#include <ncurses.h>

#include "render.h"

//int main(int argc, const char *argv[]){
int main(){
	renderInit();

	int key = getch();

	printf("%i",key);
	renderUnInit();
	return 0;
}
