#include "globals.h"
#include "log.h"
#include <stdio.h>
#include <time.h>

#ifdef DEBUG_LOG_SHOW_INGAME
#include "msg.h"
#endif

FILE *logFile;

void logInit(void){
	//Release build only keeps logs from the last game
#ifdef RELEASE_BUILD
	logFile = fopen("log.txt","w");
#else
	logFile = fopen("log.txt","a+");
#endif
	LOG_INFO("Logger started");
}

void logUnInit(void){
	LOG_INFO("Closing logger");
	fclose(logFile);
}

void insertLog(const char *prefix,const char *msg){
	fprintf(logFile,"%s %s\n",prefix,msg);
	fflush(logFile);

#ifndef RELEASE_BUILD
	MSG_ADD("%s %s",TERM_COLOR_GREEN,prefix,msg);
#endif
}
