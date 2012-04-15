#ifndef LOG_H
#define LOG_H

#define _TO_STRING(value) #value
#define TO_STRING(value) _TO_STRING(value)
#define LOG_ERROR(msg)   insertLog("ERROR: " __FILE__ ":" TO_STRING(__LINE__) " => " msg);
#define LOG_WARNING(msg) insertLog("WARNING: " __FILE__ ":" TO_STRING(__LINE__) " => " msg);
#define LOG_INFO(msg)    insertLog("INFO: " __FILE__ ":" TO_STRING(__LINE__) " => " msg);
     
void logInit(void);
void logUnInit(void);
void insertLog(char msg[500]);
#endif
