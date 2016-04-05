
extern int errno;
/* Definition of the Shared Memory attr */
#define SHM_PRM 0600
#define SHM_LEN  250

/* Definition of the Message Stack attr */
#define MSG_PRM 0600
#define TEXT_LEN 250

/* Definition of the struct of the message */
typedef struct msg_buf{
  int type;
  char text[TEXT_LEN];
} Msg;

/* Return a struct with the msg from user */
char* user_input(){
  char msg[250];
  printf("[you]> ");
  scanf("%s",msg);
  return msg;
}

/********************************
    Stack Messages functions
********************************/
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
  int status = msgsnd(qid, msg, sizeof(msg->text), 0);
  if(status < 0){
    elog("Failed to send message!");
    mlog(strerror(errno));
  }else{
    printf("Message Sended!\n");
  }
}

int read_msg(Msg* msg, int qid){
  int status = msgrcv(qid, msg, sizeof(msg->text),0,0);
  if(status < 0){
    elog("There are no messages on the queues ");
    mlog(strerror(errno));
  }
  return status;
}

void remove_queue(int qid){
  int status = msgctl(qid, IPC_RMID, NULL);
  if(status < 0){
    elog("Failed to remove queue!");
    mlog(strerror(errno));
  }
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
    printf("Shared memory %d created!\n", shmid);
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

void shm_send(Msg msg, char* shm){
  strcpy(shm, msg.text);
}

void shm_write_process(Msg msg_send){
  key_t key = 5678;
 // int shmid = shm_create(key);
  int shmid = shm_get(key);
  char* shm = shm_attach(shmid);
  msg_send.type = 0;
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
        printf("Don't detach");
        perror("shmdt");
        exit(1);
    }
  shmctl(shmid, IPC_RMID, NULL); 
}
