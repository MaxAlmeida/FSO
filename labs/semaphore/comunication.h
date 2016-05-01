#include <errno.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include "mlog.h"
/* Definition of the Shared Memory attr */
#define SHM_PRM 0600
#define SHM_LEN  250

/* Return a struct with the msg from user */
char* user_input(){
  char msg[250];
  printf("[you]> ");
  fgets(msg,sizeof(msg),stdin);
  return msg;
}
/********************************
    Shared Memory functions
********************************/
int shm_create(Sems* semaphore){
  key_t key = 5678;
  int shmid = shmget(key, sizeof(semaphore) , IPC_CREAT | 0666);
  if(shmid < 0){
    elog("Failed to create share memory!");
    mlog(strerror(errno));
  }else{
    dilog("Shared memory created! Shm ID", shmid);
  }
  return shmid;
}

int shm_get(){
  key_t key = 5678;
  int shmid = shmget(key, SHM_LEN, 0666);
  if(shmid < 0){
    mlog("No message on shared memory!");
  }
  return shmid; 
}

Sems* shm_attach(int shm_id){
  key_t key = 5678;
  Sems* shm = (Sems*) shmat(shm_id, NULL, 0);
  if(shm == (Sems *) -1){
    elog("Failed to attach memory!");
    mlog(strerror(errno));
  }
  return shm;
}

void shm_send(Sems* msg,sem_t* shm){
  strcpy(shm, msg);
}

void shm_write_process(Sems* msg_send){
  key_t key = 5678;
  int shmid = shm_get(key);
  char* shm = shm_attach(shmid);
  shm_send(msg_send,shm);
}

Sems* shm_read_process(int shm_id){
  key_t key = 5678;
  int shmid = shm_get(key);
  Sems* shm = shm_attach(shm_id); 
  return shm;
}

void clear_memmory(char* shm, int shmid){
  if (shmdt(shm) == -1) {
        dlog("Don't detach");
        exit(1);
    }
  shmctl(shmid, IPC_RMID, NULL); 
}
