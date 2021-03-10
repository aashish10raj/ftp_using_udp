
// client side 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
 
  
#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1" 
#define PORT_NO 15011 

 

  
// buffer clearance 
void clearBuf(char* b) 
{ 
    int i; 
    for (i = 0; i < 32; i++) 
        b[i] = '\0'; 
} 
  

//receive file 
int recvFile(char* buf, int s) 
{ 
    int i; 
    char ch; 
    for (i = 0; i < s; i++) { 
        ch = buf[i]; 
      
        if (ch == EOF) 
            return 1; 
        else
            printf("%c", ch); 
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
    addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS); 
    char net_buf[32]; 
    FILE* fp; 
  
    // socket() 
    sockfd = socket(AF_INET, SOCK_DGRAM, 
                    IP_PROTOCOL); 
  
    if (sockfd < 0) 
        printf("\ndescriptor not received!!\n"); 
    else
        printf("\ndescriptor %d received\n", sockfd); 
  
    while (1) { 
        printf("\nEnter file name to receive:\n"); 
        scanf("%s", net_buf); 
        sendto(sockfd, net_buf, 32, 
               0, (struct sockaddr*)&addr_con, 
               addrlen); 
  
        printf("\n-Data successfully received\n"); 
  
        while (1) { 
           
            clearBuf(net_buf); 
            nBytes = recvfrom(sockfd, net_buf, 32, 
                              0, (struct sockaddr*)&addr_con, 
                              &addrlen); 
  
            
            if (recvFile(net_buf, 32)) { 
                break; 
            } 
        } 
        printf("\n-------------------------------\n"); 
    } 
    return 0; 
} 
