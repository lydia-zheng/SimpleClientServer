#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char * argv[]) {

  int sock, portNum;
  struct sockaddr_in server;
  char message[2000], server_reply[2000];

  //Create the socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    printf("Could not create socket");
    exit(1);
  }
puts("Socket created");


portNum = 6666;
//STANDARD address for Loopback & internal testing ipv4
//inet_addr: converts string in internet standard dot notation to int value used as internet address;
server.sin_addr.s_addr = inet_addr("127.0.0.1");
server.sin_family = AF_INET;
server.sin_port = htons(portNum);

//Connect to remote server
if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
  perror("Connections failed, Error");
   exit(1);
}
puts ("Connected");

//keep communicating with server
while(1) {
  printf("Enter your message: ");
  //scanf("%s", message);  scanf doesn't read what's after space
  fgets(message, sizeof(message),stdin);

  //send some data
  if(send (sock, message, strlen(message), 0) < 0) {
    perror("Send failed");
    exit(1);
  }

  //Receive server reply
  if(recv(sock,server_reply, 2000, 0) < 0) {
    perror("recv failed...");
    break;
  }
  printf( "%s\n", server_reply);
}

close(sock);
return 0;
}