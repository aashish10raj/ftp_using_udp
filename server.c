
// server side
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

  
#define IP_PROTOCOL 0 
#define PORT_NO 15011 
#define nofile "File Not Found!" 
  
// buffer clearance
void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < 32; i++) 
        b[i] = '\0'; 
} 
  

// sending file 
int sendFile(FILE* fp, char* buf, int s) 
{ 
    int i, len; 
    if (fp == NULL) { 
        strcpy(buf, nofile); 
        len = strlen(nofile); 
        buf[len] = EOF; 
        for (i = 0; i <= len; i++) 
            buf[i] = buf[i]; 
        return 1; 
    } 
  
    char ch; 
    for (i = 0; i < s; i++) { 
        ch = fgetc(fp); 
       
        buf[i] = ch; 
        if (ch == EOF) 
            return 1; 
    } 
    return 0; 
} 
  

int main() 
{ 
    int sockfd, nBytes; 
    struct sockaddr_in addr_con; 
    int addrlen = sizeof(addr_con); 
    addr_con.sin_family = AF_INET; 
    addr_con.sin_port = htons(PORT_NO); 
    addr_con.sin_addr.s_addr = INADDR_ANY; 
    char net_buf[32]; 
    FILE* fp; 
  
    // socket() creation
    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL); 
  
    if (sockfd < 0) 
        printf("\n descriptor not received!!\n"); 
    else
        printf("\ndescriptor %d received\n", sockfd); 
  
    if (bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0) 
        printf("\nSuccessfully binded!\n"); 
    else
        printf("\nBinding Failed!\n"); 
  
    while (1) { 
        printf("\nWaiting for file name...\n"); 
  
        
        clearBuf(net_buf); 
  
        nBytes = recvfrom(sockfd, net_buf, 
                          32, 0, 
                          (struct sockaddr*)&addr_con, &addrlen); 
  
        fp = fopen(net_buf, "r"); 
        printf("\nFile Name Received: %s\n", net_buf); 
        if (fp == NULL) 
            printf("\nFile open failed!\n"); 
        else
            printf("\nFile Successfully opened!\n"); 
  
        while (1) { 
  
            
            if (sendFile(fp, net_buf, 32)) { 
                sendto(sockfd, net_buf, 32, 
                       0,  
                    (struct sockaddr*)&addr_con, addrlen); 
                break; 
            } 
  
            
            sendto(sockfd, net_buf, 32, 
                   0, 
                (struct sockaddr*)&addr_con, addrlen); 
            clearBuf(net_buf); 
        } 
        if (fp != NULL) 
            fclose(fp); 
    } 
    return 0; 
} 
