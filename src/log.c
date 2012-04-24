#include "globals.h"
#include "log.h"
#include <stdio.h>
#include <time.h>

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
	//TODO: implement time and date:q
	fprintf(logFile,"aika %s %s\n",prefix,msg);
}
