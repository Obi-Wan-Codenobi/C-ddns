#include "stdio.h"
#include "requests.h"
#include "socketConnection.c"
#include <string.h>
#include <unistd.h>


/*
 *  ~ Hi! This project is for updating my public ip address attached to my domain on a raberry pi zero.~
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

int updatePublicIpAddress(){
    char request [MAXBUF];
    const char *page = "/api/";
    const char *host = "127.0.0.1:8000";
    char *jsonMsg = 
        "{ \"comment\": \"Domain verification record\",\r\n"
            "\"content\": \"198.51.100.4\",\r\n"
            "\"name\": \"example.com\",\r\n"
            "\"proxied\": true,\r\n"
            "\"ttl\": 3600,\r\n"
            "\"type\": \"A\"\r\n"
        "}";   
    
    snprintf(request, MAXBUF, 
        "POST %s HTTP/1.0\r\n" 
        "Host: %s\r\n"    
        "Content-type: application/json\r\n"
        "X-Auth-Email: your_email@example.com\r\n"
        "X-Auth-Key: your_api_key_here\r\n"
        "Connection: close\r\n"
        "Content-length: %d\r\n\r\n"
        "%s\r\n", page, host, (unsigned int)strlen(jsonMsg), jsonMsg);
  
    char response[MAXBUF];
    apiRequest("127.0.0.1", 8000, request, response);
    return 0;
}


int main(){
    info("Starting");

    char ip [INET_ADDRSTRLEN];

    while(1){
        
        for(int i = 0; i<NUMBER_OF_DOMAINS; i++){
            if(getPublicIpAddress(ip, FIND_PUBLIC_IP_DOMAINS[i])==0){
                break;
            }
            
        }
        printf("\nip address is %s\n", ip);
        updatePublicIpAddress();
        sleep(5);
    }
    

    return 0;
}
