/*
select();
frntp();
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/msg.h>
#include<sys/errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#include "mlog.h"
#include "comunication.h"
#include "process.h"

int main(int argc, char* argv[]){

  int qid = create_queue();
  char message[250];
  printf("Enter the message: ");
  scanf("%s", message);

  Msg msg_send;
  msg_send.type = 0;
  strcpy(msg_send.text, message);
  printf(">>%s\n", &msg_send.text);
  int pid = fork();

  if(pid > 0){ // Parent
    parent_a(&msg_send, qid);
  }else if(pid == 0){ // Child
    child_a(qid);
  } else if(pid < 0){ // Fail
    printf("Failed to create process!\n", pid);
    exit(-1);
  }

} // fim-main
