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

void *printMessage(void *qid_queue){
  int qid;
  qid = (int) qid_queue;
  char temp[250];
  Msg receive_message;
  do{
    read_msg(&receive_message,qid);
    printf("<Other>: %s",receive_message.text);
    strcpy(temp,receive_message.text);  
    temp[strcspn(temp,"\n")] = '\0';
  }while(strcmp(temp,"EXIT"));

}

void *readMessage(void *qid_queue){
  int qid;
  qid = (int) qid_queue;
  
  Msg send_message;
  do{
    strcpy(send_message.text,user_input());
    send_msg(&send_message,qid);
  }while(strcmp(send_message.text,"EXIT"));
  
}
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
     // printf(">>>Recebendo mensagem do client %s\n", receive_message.text);
      //printf(">>> qid %d \n\n", params->qid);
      receive_message.type = 0;
      send_msg(&receive_message,params->qid);
    }

  }while(strcmp(receive_message.text,"EXIT"));
  close(sock_receive);
}

void *sendMessage(void *socket){
  Msg send_message;
  InfoMsg *params = socket;
  int sock_send, status;
  sock_send = params->client;
  
  do{
    int status = write(sock_send, send_message.text, sizeof(send_message.text));
    if(status < 0){
      printf("Error receiving data on thread!\n");
    }
    send_message.type = 0;
    read_msg(&send_message, params->qid);
  }while(strcmp(send_message.text,"EXIT"));
  close(sock_send);
}
