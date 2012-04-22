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

void insertLog(char *format,const char* file,const char* line,char msg[250]){
	fprintf(logFile,format,file,line,msg);
}
