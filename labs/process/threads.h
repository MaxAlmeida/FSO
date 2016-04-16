
/* Dependecies */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "pthread.h" 

/* Definition of the struct with the socket and queue */
typedef struct qid{
  int snd;
  int rcv;
} qIDs;

/* To open thread in client side */
void *client(void *qids){
  dlog(":: CLIENT ::");
  Msg msg;
  qIDs *qid = qids;
  char tmp[TEXT_LEN];
  int client = init_client();
  do{
    // Recieving
    if(select_rcv(client)){
      read(socket, msg.text, sizeof(msg.text));
      send_msg(&msg,qid->rcv);}
    // Sending
    read_msg(&msg,qid->snd);
    write(client, msg.text, sizeof(msg.text));
    strcpy(tmp,msg.text);
  }while(strcmp(tmp,"EXIT"));
}

/* To open thread in server side */
void *server(void *qids){
  dlog(":: SERVER ::");
  Msg msg;
  qIDs *qid = qids;
  char tmp[TEXT_LEN];
  int socket = init_server();
  do{
    // Recieving
    if(select_rcv(client)){
      read(socket, msg.text, sizeof(msg.text));
      send_msg(&msg,qid->rcv);}
    // Sending
    read_msg(&msg,qid->snd);
    write(client, msg.text, sizeof(msg.text));
    strcpy(tmp,msg.text);
  }while(strcmp(tmp,"EXIT"));
  close(socket);
}

/* To recieve the message from child */
void *dequeue_msg(void *qid_queue){
  int qid = (int) qid_queue;
  char tmp[TEXT_LEN];
  Msg to_rcv;
  do{
    read_msg(&to_rcv,qid);
    printf("\n\r[Other]> %s",to_rcv.text);
    strcpy(tmp,to_rcv.text);  
  }while(strcmp(tmp,"EXIT"));
}

/* To send the message to child */
void *enqueue_msg(void *qid_queue){
  int qid = (int)qid_queue;
  char tmp[TEXT_LEN];
  Msg to_snd;
  do{
    strcpy(to_snd.text,user_input());
    send_msg(&to_snd,qid);
    strcpy(tmp,to_snd.text);  
  }while(strcmp(tmp,"EXIT"));
}

/* The task for send message though SHM */
void *shm_sender(int qid){
  Msg to_send;
  char* checker = shm_read_process();
  do{
    if(!strcmp(checker,CODE)){
      read_msg(&to_send,qid);
      shm_write_process(to_send);
      dmlog("SHM has sended message!",to_send.text);
    }
  }while(strcmp(to_send.text,"EXIT"));
}

/* The task for recieve message though SHM */
void *shm_reciever(int qid){
  Msg to_rcv;
  char* shm = shm_read_process();
  do{
    sleep(1);
    if(strcmp(shm,CODE)){
      strcpy(to_rcv.text,shm);
      send_msg(&to_rcv,qid);
      dmlog("SHM has recieve message! ",to_rcv.text);
      set_checker();
    }
  }while(strcmp(to_rcv.text,"EXIT"));
}

