
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
#include "tcp.h"
#include "comunication.h"
#include "process.h"

#define DEBUG "--debug"
#define TCP "--tcp"

void flags(int argc, char* argv[]);

int main(int argc, char* argv[]){
  flags(argc,argv);
  int qid = create_queue();
  int pid = fork();

  if(pid > 0){
    parent_b(qid);
  }else if(pid == 0){
    child_b(qid);
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

