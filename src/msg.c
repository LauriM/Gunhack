#include "globals.h"
#include "msg.h"
#include "hud.h"

msg_t *head;
msg_t *end;

void msgInit(){
	head = NULL;
	end = NULL;
}

void msgAdd(char *msg,color_t color){
	if(head == NULL){
		head = malloc(sizeof(msg_t) + (strlen(msg)+1)*sizeof(char));

		head->prev = NULL;
		head->next = NULL;
		end = head;

		head->color = color;
		strcpy(head->msg,msg);

		return;
	}

	//End always points to the last node where the new nodes should be linked
	
	msg_t *newMsg = malloc(sizeof(msg_t) + (strlen(msg)+1)*sizeof(char));

	end->next = newMsg;
	newMsg->prev = end;

	newMsg->next = NULL;
	strcpy(newMsg->msg,msg);

	newMsg->color = color;
	end = newMsg;
}

void msgPrintDebugInfo(){
	msg_t *curr;

	curr = head;

	while(curr != NULL){
		LOG_INFO_F("Msg color: %i | %s",curr->color,curr->msg);
		curr = curr->next;
	}
}

void msgShowWindow(){
	hudMenuInit();
	hudMenuWrite("Message History:");
	hudMenuWrite("================");

	msg_t *curr;
	
	curr = end;

	while(curr != NULL){
		hudMenuWrite(curr->msg);
		curr = curr->prev;
	}

	hudMenuFinish();
}
