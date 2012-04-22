#ifndef LOG_H
#define LOG_H

#define _TO_STRING(value) #value
#define TO_STRING(value) _TO_STRING(value)
#define LOG_ERROR(msg) insertLog("ERROR: " __FILE__ ":" TO_STRING(__LINE__) " => " msg);
#define LOG_WARNING(msg) insertLog("WARNING: " __FILE__ ":" TO_STRING(__LINE__) " => " msg);
#define LOG_INFO(msg) insertLog("INFO: " __FILE__ ":" TO_STRING(__LINE__) " => " msg);

#ifndef DEBUG_NO_LOG_DEBUG
#define LOG_DEBUG(msg) insertLog("DEBUG: " __FILE__ ":" TO_STRING(__LINE__) " => " msg);
#else
#define LOG_DEBUG(msg) 
#endif
     
#define NEW_LOG_INFO(msg) insertLog2("INFO: %s:%s => %s \n", __FILE__,TO_STRING( __LINE__), msg)

void logInit(void);
void logUnInit(void);
void insertLog(char msg[500]);

void insertLog2(char *format,const char* file,const char* line,char msg[250]);
#endif
