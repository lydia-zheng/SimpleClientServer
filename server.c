//Reference example: https://www.binarytides.com/server-client-example-c-sockets-linux/
//                   https://www.tutorialspoint.com/unix_sockets/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> //strlen
#include <sys/socket.h> //inet_addr
#include <arpa/inet.h>
#include <unistd.h> //write
/*
SOCKET:
allows for communication between two diff processes on same/different machines
Aka. a way to talk with other computers using Unix file descriptors
*/
int main (int argc, char * argv[]) {

  int socket_fileDesc, client_sock, c, read_size, portNum;
  struct sockaddr_in server, client; //helps reference socket's elements/info
  char client_message[2000];
  
  //Create socket
  //AF_INET: internet IPv4 protocal address family 
  //SOCK_STREAM: Delivery in networked environment is guaranteed; these use TCP protocol
  //0: means let system select default based on first 2 parameters
  socket_fileDesc = socket(AF_INET, SOCK_STREAM, 0); 
  if (socket_fileDesc < 0) {
    //error case
    printf("Could not create socket");
  }
  //puts: writes str to stdout but doesn't include the null character
  //newline \n is appended to output
  puts("Socket created");



  //Initialize socket structure
  //set bytes to zero 
  bzero((char *) &server, sizeof(server));
  portNum = 6666;

  server.sin_family = AF_INET;
  //INADDR_ANY lets server's IP address be assigned automatically
  server.sin_addr.s_addr = INADDR_ANY;
  //htons: Network Byte Order; 
  //convert byte order using host -> network (short/16bit)
  //Unless using well-known port (reversed under 1024), use any int between 1024 - 65535
  server.sin_port= htons(portNum);

  //Bind
  if(bind(socket_fileDesc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    //binding failed case; print error 
    perror("bind failed. Error!");
    exit(1);
  }
  puts("bind done");

  //Listen for client side
  //3 : max # of connections the kernel should queue for this socket
  listen(socket_fileDesc, 3); 

  //Accept incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);

  //accept connection from an incoming client
  client_sock = accept(socket_fileDesc, (struct sockaddr *)&client, (socklen_t*) &c);
  if (client_sock < 0) {
    //Error case
    perror("Accept failed");
    exit(1);
  } 
  puts("Connection accepted");

  //Clean unpredicted leftover values from buffer
  bzero(client_message, sizeof(client_message));
  
  //Receive message from client side
  //client_message is the buffer that's received from client side
  //0: flag is set to 0
  while((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {

    //Send the message back to client
    write(client_sock, client_message, sizeof(client_message));
    
  }

  //read_size is non-zero while connected
  if(read_size == 0) {
    puts("Client disconnected");
    //clear/flush the buffer &move the buffered data to console
    //else it preserves old saved value and might mess with buffered data
    fflush(stdout);
  } else if (read_size == -1) {
    perror("recv failed");
  }

  return 0;

}