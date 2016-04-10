#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"pthread.h" 

/*Definition of the struct to get information the socket and queue */
typedef struct info{
  int qid;
  int client;

} InfoMsg;

void *receiveMessage(void *socket){
  Msg receive_message;
  InfoMsg *params = socket;
  int sock_receive, status;
  sock_receive = params->client;

  do{
    int status = read(sock_receive, receive_message.text, sizeof(receive_message.text));
    if(status < 0){
      printf("Error receiving data on thread!\n");
    }
    else{
      printf(">>>Recebendo mensagem do client %s\n", receive_message.text);
      printf(">>> qid %d \n\n", params->qid);
      receive_message.type = 0;
     // send_msg(&receive_message,params->qid);
    }

  }while(strcmp(receive_message.text,"EXIT"));

}
