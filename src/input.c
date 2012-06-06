#include <ncurses.h>

#include "input.h"
#include "globals.h"
#include "player.h"
#include "map.h"
#include "hud.h"
#include "save.h"
#include "npc.h"
#include "msg.h"

#include "math.h"//QQQ
#include "item.h"

inputcall_t keyBinds[256];

void input_move_n(void){
	playerMove(DIR_N);
}
void input_move_s(void){
	playerMove(DIR_S);
}
void input_move_e(void){
	playerMove(DIR_E);
}
void input_move_w(void){
	playerMove(DIR_W);
}

void input_null(void){
	return;
}

void inputInit(){
	for(int i = 0;i < 256;i++){
		keyBinds[i] = &input_null;
	}

	//TODO: Fix keypad keys
	keyBinds['l'] = &input_move_e;
	keyBinds['h'] = &input_move_w;
	keyBinds['k'] = &input_move_n;
	keyBinds['j'] = &input_move_s;
}

void inputHandle(){
	int key = getch();
	(*keyBinds[key])();
}

dir_t inputGetDirection(){
	printw("Give a direction: \n");
	int key = getch();

	switch(key){
		case 106:  
			return DIR_S;
			break;
		case 107:
			return DIR_N;
			break;
		case 108:
			return DIR_E;
			break;
		case 104:
			return DIR_W;
			break;
	}

	return DIR_S; //Add random dir with msg to player about failure to choose direction
}
