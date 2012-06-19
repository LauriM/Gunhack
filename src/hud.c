#include <ncurses.h>
#include "globals.h"
#include "hud.h"
#include "render.h"
#include "msg.h"
#include "player.h"
#include "item.h"
#include <stdio.h>

int hudPrompt(char text[250]){
	move(0,0);

	//TODO: Order of this feels weird, fix the (y/n)
	printw(text);
	//sprintf(text,"%s (y/n)",text);

	int input = getch();

	//y = 121
	//n = 110
	switch(input){
		case 121:
			return 1;
			break;
		case 110:
			return 0;
			break;
	}

	LOG_WARNING("User failed to answer y/n. Returning n");
	//Not y, return false just to be sure
	return 0;
}

void hudRender(){
	move(0,0);
	printw(msgGetLastMsg()->msg);

	char statusLine[STATUSLINE_MAX_CHARS] = "";

	snprintf(statusLine,STATUSLINE_MAX_CHARS,"LV: %i Hp: %i/%i Exp: %i/%i 9mm: %i 39mm: %i Shells: %i RPGs: %i",playerGetInfo()->pos.z,playerGetInfo()->hp,playerGetInfo()->maxhp,playerGetInfo()->exp,playerGetInfo()->level,playerGetInfo()->ammo_9mm,playerGetInfo()->ammo_39mm,playerGetInfo()->ammo_shell,playerGetInfo()->ammo_rockets);
	
	move(21,0);
	printw(statusLine);

	char* weaponName = "null";

	weaponName = itemInfo[itemData[itemGiveCurrentWpnId()].itemId].name;
	if(itemGiveCurrentWpnId() == -1){
		weaponName = "\"Unarmed\"";
	}

	snprintf(statusLine,STATUSLINE_MAX_CHARS,"%s Ammo: %i/%i Weight: %i/%i Turns: %i",weaponName,itemGetCurrentAmmoOnWeapon(),itemInfo[itemData[itemGiveCurrentWpnId()].itemId].maxAmmo,itemGetCarriedCount(),PLAYER_MAX_CARRY,playerGetInfo()->turn);

	move(22,0);
	printw(statusLine);
}

int hudMenu(const char* title,char** options,const char* prompt){
	move(0,0);//TODO: check if this call is needed
    printw(title);
	printw("\n");
	

	while(*options != NULL){
		printw(*options);
		printw("\n");
		++options;
	}
	printw("\n");
	
	printw(prompt);

	return getch();
}

void hudMenuInit(){
	move(0,0);
}

void hudMenuWrite(char *line){
	printw(line);
	printw("\n");
}

int hudMenuFinish(){
	return getch();
}
