#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORTNUM 2343
#define SA struct sockaddr


int main(int argc, char const *argv[]) {
  char msg[] = "Hello World\n";
  struct sockaddr_in dest;
  struct sockaddr_in serv;
  int mysocket;
  socklen_t socksize = sizeof(struct sockaddr_in);

  memset(&serv,0,sizeof(serv));
  serv.sin_family=AF_INET;
  serv.sin_addr.s_addr = htonl(INADDR_ANY);
  serv.sin_port = htons(PORTNUM);

  mysocket = socket(AF_INET, SOCK_STREAM, 0);
  if ((bind(mysocket, (SA*)&serv, sizeof(serv))) != 0) {
    printf("Socket bind failed..\n");
    exit(0);
  }else{
    printf("Socket bind successful\n");
  }

  if ((listen(mysocket, 5)) != 0) {
    printf("Listen failed...\n");
    exit(0);
  }else{
    printf("Server listening!\n");
  }

  int consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);
  if (consocket < 0) {
    printf("Server accept failed..\n");
    exit(0);
  }else{
    printf("Server accepted!\n");
  }

  while(consocket){
    printf("Incoming connection from %s - sending Welcome\n",inet_ntoa(dest.sin_addr));
    send(consocket, msg, strlen(msg), 0);
    consocket = accept(mysocket, (struct sockaddr*)&dest, &socksize);
  }

  close(consocket);
  close(mysocket);

  return EXIT_SUCCESS;
}
