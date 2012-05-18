#include "render.h"

typedef struct msg_s{
	struct msg_s *next;
	struct msg_s *prev;

	color_t color;
	char msg[];
} msg_t;

extern void msgInit();
extern void msgAdd(char *msg,color_t color);
extern void msgRender();

extern void msgPrintDebugInfo();

extern msg_t *head;
