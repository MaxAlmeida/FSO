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
  int pid = fork();

  if(pid > 0){ // Parent
    parent_a(qid);
  }else if(pid == 0){ // Child
    child_a(qid);
  } else if(pid < 0){ // Fail
    printf("Failed to create process!\n", pid);
    exit(-1);
  }

} // fim-main
