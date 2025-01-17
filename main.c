#include "stdio.h"
#include "requests.h"
#include "socketConnection.c"
#include <string.h>
#include <unistd.h>


/*
 *  ~ Hi! This project is for updating my public ip address attached to my domain and is used on a raberry pi zero.~
 *
 * NOTE(s): 
 *      Used https://stackoverflow.com/questions/11208299/how-to-make-an-http-get-request-in-c-without-libcurl
 *      for help with the http msg formatting and sending the data.
 *
 *      Also used some gpt for extracting the ip address from the response.
 *
 *      I additionally used class work for reference on the socket connections.
 */

int extractIpAddressFromResponse(char *response, char *ip){
    // There may be a better way to achieve this same result
    char *ipAddrStart = strstr(response, "\r\n\r\n"); 
    if (ipAddrStart) {
        // skip the "\r\n\r\n" sequence
        ipAddrStart += 4;   
        strncpy(ip, ipAddrStart, INET_ADDRSTRLEN);   
        ip[INET_ADDRSTRLEN - 1] = '\0';  
        
        //trim the pesky newline character off
        int len = strlen(ip);
        if (len > 0 && (ip[len - 1] == '\n' || ip[len - 1] == '\r')) {
            ip[len - 1] = '\0';  
        }
        return 0;
    } else {
        return -1;
    }
}

int getPublicIpAddress(char *ip, const char *host){
    char request [MAXBUF];
    const char *page = "/";
            
    snprintf(request, MAXBUF, 
        "GET %s HTTP/1.0\r\n" 
        "Host: %s\r\n"    
        "Content-type: application/json\r\n"
        "Connection: close\r\n"
        "\r\n", page, host);

    char hostIp [INET_ADDRSTRLEN];
    int ipResult = resolveDomainToIP(host, hostIp, sizeof(hostIp));
    if(ipResult!=0){
        error("Failed to fetch ip address from domain", ipResult);
        return -1;
    }
    printf("%s\n", hostIp); 
    char response[MAXBUF];
    apiRequest(hostIp, 80, request, response);
    
    return extractIpAddressFromResponse(response, ip);

}

int updatePublicIpAddress( const char *newIp, const char *name, const char *email, const char* apiKey){
    char request [MAXBUF];
    char jsonMsg [MAXBUF];
    const char *page = "/";
    const char *host = "127.0.0.1:8000";
    snprintf(jsonMsg, 1024, 
        "{ \"comment\": \"Domain verification record\", "
        "\"content\": \"%s\", "
        "\"name\": \"%s\", "
        "\"proxied\": true, "
        "\"ttl\": 3600, "
        "\"type\": \"A\" }", newIp, name);
    
    snprintf(request, MAXBUF, 
        "POST %s HTTP/1.0\r\n" 
        "Host: %s\r\n"    
        "Content-type: application/json\r\n"
        "X-Auth-Email: %s\r\n"
        "X-Auth-Key: %s\r\n"
        "Connection: close\r\n"
        "Content-length: %d\r\n\r\n"
        "%s\r\n", page, host, email, apiKey, (unsigned int)strlen(jsonMsg), jsonMsg);
  
    char response[MAXBUF];
    apiRequest("127.0.0.1", 8000, request, response);
    return 0;
}


int main(){
    info("Starting");

    char currentIp [INET_ADDRSTRLEN];

    while(1){
        char newIp [INET_ADDRSTRLEN];
        for(int i = 0; i<NUMBER_OF_DOMAINS; i++){
            if(getPublicIpAddress(newIp, FIND_PUBLIC_IP_DOMAINS[i])==0){
                break;
            }
        }

        if(strcmp(newIp, currentIp)!=0){
            strcpy(currentIp, newIp);
            printf("\nip address is %s\n", currentIp);
            updatePublicIpAddress( currentIp, "mydomain.com", "myemail@gmail.com", "API_KEY");
        }
        sleep(30);
    }
    

    return 0;
}
