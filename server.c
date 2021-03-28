#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024

void write_file(int sockfd, struct sockaddr_in addr){
  FILE *fp;
  char *filename = "b.txt";
  int n;
  char buffer[SIZE];
  socklen_t addr_size;

  // Creating a file.
  fp = fopen(filename, "w");

  // Receiving the data and writing it into the file.
  while(1){

    addr_size = sizeof(addr);
    n = recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, &addr_size);

    if (strcmp(buffer, "END") == 0){
      break;
      return;
    }

    printf("RECEVING... Data: %s", buffer);
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);

  }

  fclose(fp);
  return;
}

int main(){

  // Defining the IP and Port
  char *ip = "127.0.0.1";
  int port = 8080;

 
  int socketd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[SIZE];
  int e;

  // Creating a UDP socket
  socketd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketd < 0){
    perror("ERROR... socket error");
    exit(1);
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(socketd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (e < 0){
    perror("ERROR... bind error");
    exit(1);
  }

  printf("STARTING... UDP FTP service started. \n");
  write_file(socketd, client_addr);
  printf("\n");

  printf("SUCCESS... Data transfer complete.\n");
  printf("CLOSING... Closing the server.\n");

  close(socketd);

  return 0;
}
