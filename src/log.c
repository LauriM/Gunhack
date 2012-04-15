#include "globals.h"
#include "log.h"
#include <stdio.h>

FILE *logFile;

void logInit(void){
	logFile = fopen("log.txt","a+");
	LOG_ERROR("Logger started");
}

void logUnInit(void){
	LOG_ERROR("Closing logger");
	fclose(logFile);
}

void insertLog(char msg[500]){
	//TODO: Insert timestamp
	fprintf(logFile,"%s\n",msg);
}
