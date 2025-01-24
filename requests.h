#ifndef _REQUESTSH_
#define _REQUESTSH_

#define MAXBUF 2048
#define NUMBER_OF_DOMAINS 5


#include "logging.h"
#include "secrets.c"
const char *FIND_PUBLIC_IP_DOMAINS [NUMBER_OF_DOMAINS] = {
    "icanhazip.com",
    "ifconfig.me", 
    "ident.me", 
    "api.ipify.org", 
    "ipinfo.io.ip"
};


#endif
