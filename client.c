#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SIZE 1024

// Sending the packets
void send_file_data(FILE *fp, int sockfd, struct sockaddr_in addr){
  int n;
  char buffer[SIZE];

  
  while(fgets(buffer, SIZE, fp) != NULL){
    printf("SENDING... Data: %s", buffer);

    n = sendto(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, sizeof(addr));
    if (n == -1){
      perror("ERROR... sending data to the server.");
      exit(1);
    }
    bzero(buffer, SIZE);

  }

  // marking the END of the file
  strcpy(buffer, "END");
  sendto(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, sizeof(addr));

  fclose(fp);
  return;
}

int main(){

  // Defining the IP and Port
  char *ip = "127.0.0.1";
  int port = 8080;

  
  int socketd;
  struct sockaddr_in server_addr;
  FILE *fp;
  char *filename;
  printf("enter the name of the file you want to send to the server......");
  scanf("%s", filename);

  // Creating a UDP socket
  socketd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketd < 0){
    perror("ERROR... socket error");
    exit(1);
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  // Reading the text file
  fp = fopen(filename, "r");
  if (fp == NULL){
    perror("ERROR... while reading the file");
    exit(1);
  }

  // Sending the datagram to the server
  send_file_data(fp, socketd, server_addr);
  printf("\n");

  printf("SUCCESS... Data transfer completed.\n");
  printf("CLOSING... Logging off from the server.\n");

  close(socketd);

  return 0;

}
