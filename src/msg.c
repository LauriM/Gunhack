#include "globals.h"
#include "render.h"
#include "msg.h"
#include "player.h"
#include "hud.h"

msg_t *head = NULL;
msg_t *end  = NULL;

void msgAdd(char *msg,color_t color){
	if(head == NULL){
		head = malloc(sizeof(msg_t) + (strlen(msg)+1)*sizeof(char));

		head->prev = NULL;
		head->next = NULL;
		end = head;

		head->color = color;
		strcpy(head->msg,msg);
		head->turn = playerGetInfo()->turn;

		return;
	}

	//End always points to the last node where the new nodes should be linked
	
	msg_t *newMsg = malloc(sizeof(msg_t) + (strlen(msg)+1)*sizeof(char));

	end->next = newMsg;
	newMsg->prev = end;

	newMsg->next = NULL;
	strcpy(newMsg->msg,msg);
	newMsg->turn = playerGetInfo()->turn;

	newMsg->color = color;
	end = newMsg;

	if(msgGetMsgCount() > MSG_HISTORY_MAX_COUNT){
		msgDropHead();
	}
}

void msgDropHead(){
	msg_t *newHead;
	newHead = head->next;
	newHead->prev = head->prev;

	free(head);
	head = newHead;
}

int msgGetMsgCount(){
	int count = 0;

	msg_t *curr;
	curr = head;

	while(curr != NULL){
		count++;
		curr = curr->next;
	}

	return count;
}

void msgPrintDebugInfo(){
#ifdef DEBUG_LOG_SHOW_INGAME
	//If debugging over this is enabled, bailout from this function. It would cause a infinite loop
	LOG_WARNING("Disabled DEBUG_LOG_SHOW_INGAME to enabled msgPrintDebugInfo()");
	return;
#endif
	msg_t *curr;

	curr = head;

	while(curr != NULL){
		LOG_INFO_F("Msg color: %i | %s",curr->color,curr->msg);
		curr = curr->next;
	}
}

void msgRenderSideView(){
	int i = 0;
	msg_t *curr;

	curr = end;

	while(curr != NULL){
		setColor(curr->color);
		{
			char line[100];
			snprintf(line,100,"%i : %s",curr->turn,curr->msg);
			printxy(82,i,line);
		}
		setColorOff(curr->color);
		curr = curr->prev;
		i++;
	}
}

void msgShowWindow(){
	hudMenuInit();
	hudMenuWrite("Message History:");
	hudMenuWrite("================");

	msg_t *curr;
	
	curr = end;

	while(curr != NULL){
		setColor(curr->color);
		hudMenuWriteF("Turn %i -> %s",curr->turn,curr->msg);
		setColorOff(curr->color);
		curr = curr->prev;
	}

	hudMenuFinish();
}

void msgWriteToFile(FILE* file){
	msg_t *curr;

	curr = end;

	int hits = 0;
	while(curr != NULL){
		fprintf(file,"Turn %i -> %s\n",curr->turn,curr->msg);
		curr = curr->prev;
		++hits;

		if(hits > 15){
			return;
		}
	}
}

extern msg_t* msgGetLastMsg(){
	return end;
}
