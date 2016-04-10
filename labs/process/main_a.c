#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"pthread.h" 
#include<sys/socket.h>
#include<sys/msg.h>
#include<sys/errno.h>
#include<sys/ipc.h>
#include<sys/shm.h>

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
  Msg msg_check;
  key_t key = KEY;
  int shmid = shm_create(key);
  strcpy(msg_check.text, CODE);
  shm_write_process(msg_check);
  if(pid > 0){ // Parent
    parent_a(qid);
  }else if(pid == 0){ // Child
    child_a(qid);
  } else if(pid < 0){ // Fail
    printf("Failed to create process!\n", pid);
    exit(-1);
  }

} // fim-main

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

