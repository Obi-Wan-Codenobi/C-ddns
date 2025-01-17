#ifndef _LOGGINGH_
#define _LOGGINGH_
#include <stdbool.h>
#include "stdio.h"
#include "stdlib.h"

void printLog( const char *msg, const char *type){
    printf("[%s] -> %s\n", type, msg);
}

void error(const char *msg, bool flag){
    printLog(msg, "ERROR");
}

void info(const char *msg){
    printLog(msg, "INFO");
}

void debug(const char *msg, const char* functionName){
    // Need to concatenate the 'DEBUG - functionName' 
    printLog(msg, functionName);
}

#endif
