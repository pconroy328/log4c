/*
 */

/* 
 * File:   log4c.h
 * Author: pconroy
 *
 * Created on May 13, 2019, 1:35 PM
 */

#ifndef LOG4C_H
#define LOG4C_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdarg.h>
    
#ifndef  FALSE
#define FALSE 0
#define TRUE  (!FALSE)
#endif
// ----------------------------------------------------------------------------
//
// How "debugValue" works.  It's an integer
// 0 = Log nothing         
// 5 = Log Fatal and Error and Warning and Debug and Info
// 4 = Log Fatal and Error and Warning and Debug
// 3 = Log Fatal and Error and Warning
// 2 = Log Fatal and Error
// 1 = Log Fatal
    
#define LOG_FATAL                               1
#define LOG_FATAL_ERROR                         2
#define LOG_FATAL_ERROR_WARNING                 3
#define LOG_FATAL_ERROR_WARNING_DEBUG           4
#define LOG_FATAL_ERROR_WARNING_DEBUG_INFO      5
#define LOG_NOTHING                             0

#define LOG_ERROR                               LOG_FATAL_ERROR
#define LOG_WARNING                             LOG_FATAL_ERROR_WARNING
#define LOG_DEBUG                               LOG_FATAL_ERROR_WARNING_DEBUG
#define LOG_INFO                                LOG_FATAL_ERROR_WARNING_DEBUG_INFO
    
    
    

    extern void Logger_Initialize(char *fileName, int debugValue);
    extern void Logger_Terminate();
    extern void Logger_LogInfo_X(const char *fname, const int lineNum, const char *func, char *format, ...);
    extern void Logger_LogDebug_X(const char *fname, const int lineNum, const char *func, char *format, ...);
    extern void Logger_LogWarning_X(const char *fname, const int lineNum, const char *func, char *format, ...);
    extern void Logger_LogError_X(const char *fname, const int lineNum, const char *func, char *format, ...);
    extern void Logger_LogFatal_X(const char *fname, const int lineNum, const char *func, char *format, ...);


#define Logger_FunctionStart(x)     Logger_LogDebug( "%s[%d] :: %s() - enter\n", __FILE__, __LINE__, __func__)
#define Logger_FunctionEnd(x)       Logger_LogDebug( "%s[%d] :: %s() - exit\n", __FILE__, __LINE__, __func__ )


#define Logger_LogDebug(...)        Logger_LogDebug_X(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define Logger_LogInfo(...)         Logger_LogInfo_X(__FILE__, __LINE__,  __func__, __VA_ARGS__)
#define Logger_LogWarning(...)      Logger_LogWarning_X(__FILE__, __LINE__,  __func__, __VA_ARGS__)
#define Logger_LogError(...)        Logger_LogError_X(__FILE__, __LINE__,  __func__, __VA_ARGS__)
#define Logger_LogFatal(...)        Logger_LogFatal_X(__FILE__, __LINE__,  __func__, __VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif /* LOG4C_H */

