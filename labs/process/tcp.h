#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"pthread.h"

#define IP "127.0.0.1"
#define PORT 8502

/* Creating a Socket */
int create_socket(){
  int status = socket(AF_INET, SOCK_STREAM,0);
  if(status < 0) elog("Failed to open socket!");
  dlog("Socket created!");
  return status;
}

/* Bind the Port */
void bind_port( int socket,struct sockaddr_in server ){
  int status = bind(socket,(struct sockaddr *)&server, sizeof(server));
  if( status < 0) elog("Failed to bind!");
  dlog("Bind Done!"); 
}

/* Connect to the server */
void set_connection(int socket, struct sockaddr_in server){
  int status = connect(socket,(struct sockaddr *)&server, sizeof(server));
  if(status < 0) elog("Failed to connect!");
  dlog("Connected!");
}

/* Build a sockaddr structure */
struct sockaddr_in init_sockaddr(int port, char* ip){
  struct sockaddr_in socket;
  socket.sin_family = AF_INET;
  socket.sin_port = htons(port);
  socket.sin_addr.s_addr = inet_addr(ip);
  return socket;
}

/* Use to init a server side into the program */
int init_server(){
  int socket = create_socket();
  socklen_t cli_len = sizeof(struct sockaddr_in);

  struct sockaddr_in client;
  struct sockaddr_in server = init_sockaddr(PORT, IP);

  bind_port(socket, server);
  printf("\nWaiting for connection...\n");
  listen(socket,3);

  int new_cli = accept(socket, (struct sockaddr *)&client, &cli_len);
  if(new_cli < 0) elog("Failed to accept!");
  printf("\nConnection accepted!\n");

  return new_cli;
}

/* Used to init a client side into the program */
int init_client(){
  int socket = create_socket();
  struct sockaddr_in server = init_sockaddr(PORT, IP);
  set_connection(socket, server);
  return socket;
}

