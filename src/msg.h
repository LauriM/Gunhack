#include "render.h"

#ifndef MSG_H
#define MSG_H

typedef struct msg_s{
	struct msg_s *next;
	struct msg_s *prev;

	color_t color;
	int turn;
	char msg[];
} msg_t;

void msgInit();
void msgAdd(char *msg,color_t color);
void msgShowWindow();
void msgWriteToFile(FILE* file);
void msgDropHead();
msg_t* msgGetLastMsg();
int msgGetMsgCount();
void msgRenderSideView();

void msgPrintDebugInfo();

extern msg_t *head;
extern msg_t *end;

#define MSG_ADD(p_msg,p_color,...) {char output[100]; snprintf(output,100,p_msg,__VA_ARGS__);msgAdd(output,p_color);}

#endif
