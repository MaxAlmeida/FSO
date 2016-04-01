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
  int pid = fork();

  if(pid > 0){
    parent_b(qid);
  }else if(pid == 0){
    child_b(qid);
  }else if(pid < 0){
    printf("Failed to create process!\n",pid);
    exit(-1);
  } 
  //key_t key = 5678;
  //int mem_id = shm_get(key);
  //char* shm = shm_attach(mem_id);
  //printf(">>>> %d\n\n",mem_id);
  // printf("Lido na memoria: %s\n",shm);


}
