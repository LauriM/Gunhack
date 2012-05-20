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

extern void msgInit();
extern void msgAdd(char *msg,color_t color);
extern void msgShowWindow();
extern void msgDropHead();
extern msg_t* msgGetLastMsg();
extern int msgGetMsgCount();
extern void msgRenderSideView();

extern void msgPrintDebugInfo();

extern msg_t *head;
extern msg_t *end;

#define MSG_ADD(p_msg,p_color,...) {char output[100]; snprintf(output,100,p_msg,__VA_ARGS__);msgAdd(output,p_color);}

#endif
