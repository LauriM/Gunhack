#include "../globals.h"
#include "../msg.h"
#include "../item.h"

void itemCall_null(int itemId,itemaction_t action){
	msgAdd("You can't use this item!",TERM_COLOR_DEFAULT);
	return;
}
