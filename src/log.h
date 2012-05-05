#ifndef LOG_H
#define LOG_H

#include "globals.h"

#define LOG_ERROR(msg) insertLog("ERROR: " TO_STRING(__FILE__) ":" TO_STRING( __LINE__), msg) 
#define LOG_WARNING(msg) insertLog( "WARNING: " TO_STRING(__FILE__) ":" TO_STRING( __LINE__), msg) 
#define LOG_INFO(msg) insertLog( "INFO: " TO_STRING(__FILE__) ":" TO_STRING( __LINE__), msg) 

#ifndef DEBUG_NO_LOG_DEBUG
#define LOG_DEBUG(msg) insertLog("DEBUG: " TO_STRING(__FILE__) ":" TO_STRING( __LINE__), msg) 
#else
#define LOG_DEBUG(msg) 
#endif
     
void logInit(void);
void logUnInit(void);
void insertLog(const char *prefix,const char *msg);

#endif
