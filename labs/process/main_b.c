
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<sys/msg.h>
#include<sys/errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include"pthread.h"
#include "mlog.h"
#include "comunication.h"
#include "tcp.h"
#include "threads.h"
#include "process.h"

#define DEBUG "--debug"
#define TCP "--tcp"

void flags(int argc, char* argv[]);

int main(int argc, char* argv[]){
  flags(argc,argv);
  int qid_receive = create_queue();
  int qid_send = create_queue();
  int pid = fork();

  if(pid > 0){
    parent_b(qid_send, qid_receive);
  }else if(pid == 0){
    child_b(qid_send, qid_receive);
  }else if(pid < 0){
    printf("Failed to create process!\n",pid);
    exit(-1);
  } 
}
void flags(int argc, char* argv[]){
  int i = 0;
  if(argc > 1){
    for(i=1;i<argc;i++){
      if(!strcmp(DEBUG,argv[i]))
        EN_LOG = true;
      if(!strcmp(TCP,argv[i]))
        EN_TCP = true;
    }
  }
}

