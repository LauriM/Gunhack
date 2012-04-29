#ifndef RENDER_H
#define RENDER_H

typedef enum{
	TERM_COLOR_DEFAULT = 0,
	TERM_COLOR_RED,
	TERM_COLOR_GREEN,
	TERM_COLOR_YELLOW,
	TERM_COLOR_BLUE,
	TERM_COLOR_MAGENTA,
	TERM_COLOR_CYAN,
	TERM_COLOR_WHITE_RED,
	TERM_COLOR_WHITE_GREEN
	//TODO: rest of the colors...
} color_t;

extern void renderInit(void);
extern void renderUnInit(void);

extern void printxy(int x,int y,char* ch);
extern void printIntxy(int x,int y,int i);

extern void setColor(color_t colorCode);
extern void setColorOff(color_t colorCode);

#endif
