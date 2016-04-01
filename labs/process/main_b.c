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
  
  key_t key = 5678;
  int mem_id = shm_get(key);
  char* shm;
  if ((shm = shmat(mem_id, (void *)0, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
  printf(">>>> %d\n\n",mem_id);
  printf("Lido na memoria: %s\n",shm);
 

}
