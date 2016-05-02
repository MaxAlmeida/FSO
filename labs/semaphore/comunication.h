#include <errno.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include "mlog.h"

/* Definition of the Shared Memory attr */
#define SHM_PRM 0600
#define SHM_LEN  250
#define KEY     5678

/* Defining the Structure of semaphore */
typedef struct sems{
  sem_t mutex;
  sem_t senador;
  int qnt_s;
  int qnt_d;
  int qnt_v;
  sem_t parlamentar[];
}Sems;

/********************************
    Shared Memory functions
********************************/
int shm_create(Sems* semaphore){
  key_t key = KEY;
  int shmid = shmget(key, sizeof(semaphore) , IPC_CREAT | 0666);
  if(shmid < 0){
    elog("Failed to create share memory!");
  }else{
    dilog("Shared memory created! Shm ID", shmid);
  }
  return shmid;
}

int shm_get(){
  key_t key = KEY;
  int shmid = shmget(key, SHM_LEN, 0666);
  if(shmid < 0){
    mlog("No message on shared memory!");
  }
  return shmid; 
}

Sems* shm_attach(int shm_id){
  key_t key = KEY;
  Sems* shm = (Sems*) shmat(shm_id, NULL, 0);
  if(shm == (Sems *) -1){
    elog("Failed to attach memory!");
  }
  return shm;
}

void shm_send(Sems* msg,char* shm){
  strcpy(shm, msg);
}

void shm_write_process(Sems* msg_send){
  key_t key = KEY;
  int shmid = shm_get(key);
  char* shm = shm_attach(shmid);
  shm_send(msg_send,shm);
}

Sems* shm_read_process(int shm_id){
  key_t key = KEY;
  int shmid = shm_get(key);
  Sems* shm = shm_attach(shm_id); 
  return shm;
}

void clear_memmory(char* shm, int shmid){
  if (shmdt(shm) == -1) dlog("Don't detach");
  shmctl(shmid, IPC_RMID, NULL); 
}
