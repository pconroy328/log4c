/*
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>


#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

#include "log4c.h"



static  FILE    *fp;
static  char    *currentFileName;
static  int     logFileOpen = FALSE;
static  int     debugValue;                     // from the INI file
static  int     padCategory = TRUE;

static  pthread_mutex_t     mutex   = PTHREAD_MUTEX_INITIALIZER;
;

//
// Forward declarations
static  char    *getCurrentDateTime( char *buffer, int buffLen );



// ----------------------------------------------------------------------------
//
// How "debugValue" works.  It's an integer
// 0 = Log nothing         
// 5 = Log Fatal and Error and Warning and Debug and Info
// 4 = Log Fatal and Error and Warning and Debug
// 3 = Log Fatal and Error and Warning
// 2 = Log Fatal and Error
// 1 = Log Fatal

// ----------------------------------------------------------------------------
void    Logger_Initialize (char *fileName, int debugLevel)
{
    char    dateTimeBuffer[ 80 ];
    
    printf( "ATTEMPTING TO OPEN [%s] DEBUG LEVEL [%d]\n", fileName, debugLevel ); fflush( stdout );
    debugValue = debugLevel;
    currentFileName = fileName;
  
    //
    // Remember UNIX filesystems often do not keep a "Creation Date" for a file.
    // At best you'll have 'accessed (read)', 'modified (written to)', changed...
    // 
    // So we need to figure out a way to determine if the file we're about to open should be
    // rolled before we open it.
    //

    
    fprintf( stderr, "[%s] Logger_Initialize() called - Opening up the file.\n", getCurrentDateTime( dateTimeBuffer , sizeof dateTimeBuffer ) );
    if (fileName != (char *) 0 ) {
        fp = fopen( fileName, "a" );
        if (fp != (FILE *) 0) {
            logFileOpen = TRUE;
        
        }
    }
    
    if (logFileOpen)
        fprintf( stderr, "[%s] Logger_Initialize() called - Log File is open.\n", getCurrentDateTime( dateTimeBuffer , sizeof dateTimeBuffer ) );  
}

// ----------------------------------------------------------------------------
void    Logger_UpdateDebugLevel (int newLevel)
{
    debugValue = newLevel;
}


// ----------------------------------------------------------------------------
void    Logger_Terminate()
{
    if (logFileOpen)
        fclose( fp );
    pthread_mutex_destroy( &mutex );
}

// ----------------------------------------------------------------------------
void    Logger_LogDebug_X (const char *fname, const int lineNum, const char *func, char *format, ... )
{
    char    dateTimeBuffer[ 80 ];
    
    if (!logFileOpen || debugValue < 4)
        return;
    
    pthread_mutex_lock( &mutex );
    va_list args;
    
    va_start( args, format );                   // the last fixed parameter
    int numWritten = fprintf( fp, "DEBUG  |%s|%s|%s:%d|", getCurrentDateTime( dateTimeBuffer , sizeof dateTimeBuffer ), fname, func, lineNum );
    numWritten += vfprintf( fp, format, args );
    va_end( args );
    
    fflush( fp );
    pthread_mutex_unlock( &mutex );
}

// ----------------------------------------------------------------------------
void    Logger_LogWarning_X (const char *fname, const int lineNum, const char *func, char *format, ...)
{
    char    dateTimeBuffer[ 80 ];
    
    if (!logFileOpen || debugValue < 3)
        return;
    
    pthread_mutex_lock( &mutex );
    va_list args;
    
    va_start( args, format );                   // the last fixed parameter
    int numWritten = fprintf( fp, "WARNING|%s|%s|%s:%d|", getCurrentDateTime( dateTimeBuffer , sizeof dateTimeBuffer ), fname, func, lineNum );
    numWritten += vfprintf( fp, format, args );
    va_end( args );
    
    fflush( fp );   
    pthread_mutex_unlock( &mutex );
}

// ----------------------------------------------------------------------------
void    Logger_LogError_X (const char *fname, const int lineNum, const char *func, char *format, ...)
{
    char    dateTimeBuffer[ 80 ];
    
    if (!logFileOpen || debugValue < 2)
        return;
    
    pthread_mutex_lock( &mutex );
    va_list args;
    
    va_start( args, format );                   // the last fixed parameter
    int numWritten = fprintf( fp, "ERROR  |%s|%s|%s:%d|", getCurrentDateTime( dateTimeBuffer , sizeof dateTimeBuffer ), fname, func, lineNum );
    numWritten += vfprintf( fp, format, args );
    va_end( args );
    
    fflush( fp );   
    pthread_mutex_unlock( &mutex );
}

// ----------------------------------------------------------------------------
void    Logger_LogFatal_X (const char *fname, const int lineNum, const char *func, char *format, ...)
{
    char    dateTimeBuffer[ 80 ];
    
    if (!logFileOpen || debugValue < 1)
        return;
    
    pthread_mutex_lock( &mutex );
    va_list args;
    
    va_start( args, format );                   // the last fixed parameter
    int numWritten = fprintf( fp, "FATAL  |%s|%s|%s:%d|", getCurrentDateTime( dateTimeBuffer , sizeof dateTimeBuffer ), fname, func, lineNum );
    numWritten += vfprintf( fp, format, args );
    va_end( args );

    fprintf( fp, "FATAL  |%s|%s|%s:%d|PROGRAM TERMINATION\n", getCurrentDateTime( dateTimeBuffer , sizeof dateTimeBuffer ), fname, func, lineNum );
    fflush( fp );
    
    pthread_mutex_unlock( &mutex );
    Logger_Terminate();
    exit( 1 );
}

// ----------------------------------------------------------------------------
void    Logger_LogInfo_X (const char *fname, const int lineNum, const char *func, char *format, ...)
{
    char    dateTimeBuffer[ 80 ];
    
    if (!logFileOpen || debugValue < 5)
        return;

    pthread_mutex_lock( &mutex );
    va_list args;
    
    va_start( args, format );                   // the last fixed parameter
    int numWritten = fprintf( fp, "%-7s|%s|%s|%s:%d|", 
            "INFO",
            getCurrentDateTime( dateTimeBuffer , sizeof dateTimeBuffer ),
            fname, func, lineNum);


    numWritten += vfprintf( fp, format, args );
    va_end( args );
    
    fflush( fp );
    pthread_mutex_unlock( &mutex );
}

// -----------------------------------------------------------------------------
static
char    *getCurrentDateTime (char *buffer, int buffLen)
{
    //
    // Something quick and dirty... Fix this later - thread safe
    time_t  current_time;
    struct  tm  *tmPtr;
 
    memset( buffer, '\0', buffLen );
    
    /* Obtain current time as seconds elapsed since the Epoch. */
    current_time = time( NULL );
    if (current_time > 0) {
        /* Convert to local time format. */
        tmPtr = localtime( &current_time );
 
        if (tmPtr != NULL) {
            strftime( buffer,
                    buffLen,
                    "%F %T %z",
                    tmPtr );
            
        }
    }
    
    return buffer;
}


// ----------------------------------------------------------------------------
static
void    Logger_LogIt (const char *fname, const int lineNum, const char *func, const char *level, char *format, ...)
{
    char    dateTimeBuffer[ 80 ];
    
    if (!logFileOpen || debugValue < 5)
        return;
    
}
