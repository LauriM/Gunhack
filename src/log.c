#include "globals.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

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
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%H:%M:%S",timeinfo);

	fprintf(logFile,"%s %s\n",buffer,msg);
}
