#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

extern int errno;
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
int shm_create(key_t key){
  int shmid = shmget(key, SHM_LEN, IPC_CREAT | 0666);
  if(shmid < 0){
    elog("Failed to create share memory!");
    mlog(strerror(errno));
  }else{
    dilog("Shared memory created! Shm ID", shmid);
  }
  return shmid;
}

int shm_get(key_t key){
  int shmid = shmget(key, SHM_LEN, 0666);
  if(shmid < 0){
    mlog("No message on shared memory!");
  }
  return shmid; 
}

char* shm_attach(int shmid){
  char* shm = shmat(shmid, NULL, 0);
  if(shm == (char *) -1){
    elog("Failed to attach memory!");
    mlog(strerror(errno));
  }
  return shm;
}

void shm_send(char msg,char* shm){
  strcpy(shm, msg);
}

void shm_write_process(char msg_send){
  key_t key = 5678;
  int shmid = shm_get(key);
  char* shm = shm_attach(shmid);
  shm_send(msg_send,shm);
}

char* shm_read_process(){
  key_t key = 5678;
  int shmid = shm_get(key);
  char* shm = shm_attach(shmid); 
 return shm;
}

void clear_memmory(char* shm, int shmid){
  if (shmdt(shm) == -1) {
        dlog("Don't detach");
        exit(1);
    }
  shmctl(shmid, IPC_RMID, NULL); 
}
