#ifndef LOG_H
#define LOG_H

#include "globals.h"

#define LOG_ERROR(msg) insertLog("ERROR: " TO_STRING(__FILE__) ":" TO_STRING( __LINE__), msg) 
#define LOG_ERROR_F(p_msg,...) {char output[100]; snprintf(output,100,p_msg,__VA_ARGS__);LOG_ERROR(output);}
#define LOG_WARNING(msg) insertLog( "WARNING: " TO_STRING(__FILE__) ":" TO_STRING( __LINE__), msg) 
#define LOG_WARNING_F(p_msg,...) {char output[100]; snprintf(output,100,p_msg,__VA_ARGS__);LOG_WARNING(output);}
#define LOG_INFO(msg) insertLog( "INFO: " TO_STRING(__FILE__) ":" TO_STRING( __LINE__), msg) 
#define LOG_INFO_F(p_msg,...) {char output[100]; snprintf(output,100,p_msg,__VA_ARGS__);LOG_INFO(output);}

#ifndef DEBUG_NO_LOG_DEBUG
#define LOG_DEBUG(msg) insertLog("DEBUG: " TO_STRING(__FILE__) ":" TO_STRING( __LINE__), msg) 
#define LOG_DEBUG_F(p_msg,...) {char output[100]; snprintf(output,100,p_msg,__VA_ARGS__);LOG_DEBUG(output);}
#else
#define LOG_DEBUG(msg) 
#define LOG_DEBUG_F(p_msg,p_value)
#endif

void logInit(void);
void logUnInit(void);
void insertLog(const char *prefix,const char *msg);

#endif
