#include "stdio.h"
#include "requests.h"
#include "httpsRequest.c"
#include "httpRequest.c"
#include <stdbool.h>
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
 *      Mainly from Texas A&M University CSCE 464 w/ Dr.Radu Stoleru.
 *
 *      I also decided to not allocate memory on heap as much as possible, so the 
 *      buffer size should be allocated appropriately for the data you will be 
 *      handling
 */

int extractIpAddressFromResponse(char *response, char *ip){
    // There may be a better way to achieve this same result
    char *ipAddrStart = strstr(response, "\r\n\r\n"); 
    if (ipAddrStart) {
        // skip the "\r\n\r\n" sequence in a http response
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
    char response[MAXBUF];
    httpRequest(hostIp, 80, request, response);
    
    return extractIpAddressFromResponse(response, ip);

}

int updatePublicIpAddress(const char *host, const char *newIp, const char *name, const char *email,
        const char* apiKey, const char *zoneId, const char *dnsRecordId) {

    char request [MAXBUF];
    char jsonMsg [MAXBUF];
    //const char *host = "127.0.0.1:8000";
    char page [MAXBUF];
    // snprintf(page, MAXBUF, 
    //     "/");


    int bytesLen = snprintf(page, MAXBUF, 
       "/client/v4/zones/%s/dns_records/%s"
        , zoneId, dnsRecordId);

    if (bytesLen > MAXBUF) {
	fprintf(stderr, "Error: Buffer overflow for page.\n");
    	return -1;
    }

    bytesLen = snprintf(jsonMsg, MAXBUF, 
        "{ \"comment\": \"Domain verification record\", "
        "\"content\": \"%s\", "
        "\"name\": \"%s\", "
        "\"proxied\": true, "
        "\"ttl\": 3600, "
        "\"type\": \"A\" }", newIp, name);
    
    if (bytesLen > MAXBUF) {
	fprintf(stderr, "Error: Buffer overflow for jsonMsg.\n");
    	return -1;
    }

    bytesLen = snprintf(request, MAXBUF, 
        "PUT %s HTTP/1.0\r\n" 
        "Host: %s\r\n"    
        "Content-type: application/json\r\n"
        "X-Auth-Email: %s\r\n"
        "X-Auth-Key: %s\r\n"
        "Connection: close\r\n"
        "Content-length: %d\r\n\r\n"
        "%s\r\n", page, host, email, apiKey, (unsigned int)strlen(jsonMsg), jsonMsg);
  
    if (bytesLen > MAXBUF) {
	fprintf(stderr, "Error: Buffer overflow for request.\n");
    	return -1;
    }
    char response[MAXBUF];
    char hostIp [INET_ADDRSTRLEN];
    int ipResult = resolveDomainToIP(host, hostIp, sizeof(hostIp));
    if(ipResult!=0){
        error("Failed to fetch ip address from domain", ipResult);
        return -1;
    }
    
    return httpsRequest(host, 443 , request, response);
}


int main(){
    info("Starting c-ddns . . .");

    char currentIp [INET_ADDRSTRLEN];
    int success = -1;

    while(1){
        char newIp [INET_ADDRSTRLEN];
        for(int i = 0; i<NUMBER_OF_DOMAINS; i++){
            if(getPublicIpAddress(newIp, FIND_PUBLIC_IP_DOMAINS[i])==0){
                break;
            }
        }

        if( (strcmp(newIp, currentIp)!=0) || success<0){

            strcpy(currentIp, newIp);
            char newIpAddressMsg [MAXBUF];
            snprintf(newIpAddressMsg, MAXBUF, "New Ip Address: [%s]", currentIp);
            info(newIpAddressMsg);

            success = updatePublicIpAddress(CLOUD_FLARE_DOMAIN, currentIp, DOMAIN, EMAIL,
                    CLOUD_FLARE_API_KEY, ZONEID, DNS_RECORD_ID);
            if (success<0){
                error("FAILED TO CONTACT SERVER", 1);
                sleep(3);
                continue;
            }
        }
        sleep(30);
    }
    

    return 0;
}
