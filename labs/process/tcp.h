#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

bool EN_TCP = false;

int create_socket(){
  int status = socket(AF_INET, SOCK_STREAM,0);
  if(status < 0) elog("Failed to socket!");
  return status;
}

void bind_port( int socket,struct sockaddr_in server ){
  int status = bind(socket,(struct sockaddr *)&server, sizeof(server));
  if( status < 0){
    elog("Failed to bind!");
  }
  dlog("Bind Done!"); 
}

int set_connection(int socket, struct sockaddr_in server){
  int status = connect(socket,(struct sockaddr *)&server, sizeof(server));
  if(status < 0)
    elog("Failed to connect!");
  dlog("Connected!");
}

struct sockaddr_in init_sockaddr(int port){
  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);

  return server;
}

void init_server(){
  // TCP connection
  struct sockaddr_in client;
  struct sockaddr_in server = init_sockaddr(8200);
  int socket = create_socket();
  bind_port(socket, server);

  printf("Waiting for connection...\n");
  listen(socket,3);

  int status = sizeof(struct sockaddr_in);
  int accp = accept(socket, (struct sockaddr *)&client, (socklen_t*)&status);
  if(accp < 0) elog("Failed to accept!");
  printf("Connection accepted!\n");
}

void init_client(){
  struct sockaddr_in server = init_sockaddr(8200);
  int socket = create_socket();
  set_connection(socket, server);
}
