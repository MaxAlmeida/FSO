
extern int errno;
#define SHM_PRM 0600
#define MSG_PRM 0600

#define SHM_LEN  250
#define TEXT_LEN 250

int mem_id;
typedef struct msg_buf{
  int type;
  char text[TEXT_LEN];
} Msg;

/* Stack Messages functions */
int create_queue(){
  int qid = msgget(IPC_PRIVATE, MSG_PRM | IPC_CREAT | IPC_EXCL);
  if(qid < 0){
    elog("Failed to create queue!");
    mlog(strerror(errno));
  } else {
    printf("Message queue %d created!\n", qid);
  }
  return qid;
}

void send_msg(Msg* msg, int qid){
  int result = 0;
  int status = msgsnd(qid, msg, sizeof(msg->text), 0);
  if(status < 0){
    elog("Failed to send message!");
    mlog(strerror(errno));
  }else{
    printf("Message Sended!\n");
  }
}

void read_msg(Msg* msg, int qid){
  int status = msgrcv(qid, msg, sizeof(msg->text),0,0);
  if(status < 0){
    elog("Failed to receive message!");
    mlog(strerror(errno));
  }
}

void remove_queue(int qid){
  int status = msgctl(qid, IPC_RMID, NULL);
  if(status < 0){
    elog("Failed to remove queue!");
    mlog(strerror(errno));
  }
}

/* Shared Memory functions */
int shm_create(key_t key){
  int shmid = shmget(key, SHM_LEN, IPC_CREAT | 0666);
  if(shmid < 0){
    elog("Failed to create share memory!");
    mlog(strerror(errno));
  }else{
    printf("Shared memory %d created!\n", shmid);
  }
  return shmid;
}

int shm_get(key_t key){
  int shmid = shmget(key, SHM_LEN, 0666);
  if(shmid < 0){
    elog("Failed to create share memory!");
    mlog(strerror(errno));
  }else{
    printf("Shared memory %d created!\n",shmid);
  }
  return shmid; 
}

char* shm_attach(int shmid){
  printf("executando atachh\n\n");
  char* shm = shmat(shmid, NULL, 0);
  if(shm == (char *) -1){
    elog("Failed to attach memory!");
    mlog(strerror(errno));
  }
  return shm;
}

void shm_send(Msg msg, char* shm){
  //strcpy(shm, "funfaaaa");
  strcpy(shm, msg.text);
  printf("Ataaaaaach  %s\n\n",shm);
}

void shm_process(Msg msg_send){
  key_t key = 5678;
  int shmid = shm_create(key);
  mem_id = shmid; 
  char* shm = shm_attach(shmid);

  //Msg msg_send;
  //strcpy(msg_send.text,"YEAH!");
  msg_send.type = 0;
  shm_send(msg_send,shm);
}

int return_mem_id(){
  return mem_id;
}
