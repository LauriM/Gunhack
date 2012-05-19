#include "globals.h"
#include "log.h"
#include <stdio.h>
#include <time.h>

#ifdef DEBUG_LOG_SHOW_INGAME
#include "msg.h"
#endif

FILE *logFile;

void logInit(void){
	logFile = fopen("log.txt","a+");
	LOG_INFO("Logger started");
}

void logUnInit(void){
	LOG_INFO("Closing logger");
	fclose(logFile);
}

void insertLog(const char *prefix,const char *msg){
	fprintf(logFile,"%s %s\n",prefix,msg);
	fflush(logFile);

#ifdef DEBUG_LOG_SHOW_INGAME
	MSG_ADD("%s %s",TERM_COLOR_GREEN,prefix,msg);
#endif
}
