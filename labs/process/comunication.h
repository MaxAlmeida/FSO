
extern int errno;
/* Definition of the Shared Memory attr */
#define SHM_PRM 0666
#define SHM_LEN  250
#define KEY     5678

/* Definition of the Message Stack attr */
#define MSG_PRM  0600
#define TEXT_LEN  250

/* Definition of the struct of the message
   It's used in the Queue IPC */
typedef struct msg_buf{
  int type;
  char text[TEXT_LEN];
} Msg;

/* Read the message form the user
    @return msg, the input message from user*/
char* user_input(){
  char msg[250];
  printf("[you]> ");
  fgets(msg,sizeof(msg),stdin);
  return msg;
}

/********************************
    Stack Messages functions
********************************/

/* Creates a new queue of message
    @return qid, the ID of the queue */
int create_queue(){
  int qid = msgget(IPC_PRIVATE, MSG_PRM | IPC_CREAT | IPC_EXCL);
  if(qid < 0){
    elog("Failed to create queue!");
    mlog(strerror(errno));
  } else {
    dilog("Message queue created! Queue ID", qid);
  }
  return qid;
}

/* Enqueue the message
    @param msg, structure with msg and type of the msg
    @param qid, the ID of the queue to send */
void send_msg(Msg* msg, int qid){
  int status = msgsnd(qid, msg, sizeof(msg->text), 0);
  if(status < 0){
    elog("Failed to send message!");
    mlog(strerror(errno));
  }else{
    dlog("Message Sended!\n");
  }
}

/* Dequeue the message
    @param *msg, the struct to write the message from queue
    @param qid, the ID of the queue to recieve */
int read_msg(Msg* msg, int qid){
  int status = msgrcv(qid, msg, sizeof(msg->text),0,0);
  if(status < 0){
    elog("There are no messages on the queues!");
    mlog(strerror(errno));
  }
  return status;
}

/* Remove the queue messge
    @param qid, the ID of the queue to remove */
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

/* Creates the shared memory
    @return shmid, the ID of the shared memory*/
int shm_create(){
  int shmid = shmget(KEY, SHM_LEN, IPC_CREAT | SHM_PRM);
  if(shmid < 0){
    elog("Failed to create share memory!");
    mlog(strerror(errno));
  }else{
    dilog("Shared memory created! Shm ID", shmid);
  }
  return shmid;
}

/* Return the ID of the shared memory
    @return shmid, the ID of shared memory*/
int shm_get(){
  int shmid = shmget(KEY, SHM_LEN, SHM_PRM);
  if(shmid < 0){
    dlog("No message on shared memory!");
  }else{
  }
  return shmid; 
}

/* Gets the message from a memory attach
    @param shmid, the ID of the shared memory
    @return shm, the */
char* shm_attach(int shmid){
  char* shm = shmat(shmid, NULL, 0);
  if(shm == (char *) -1){
    elog("Failed to attach memory!");
    mlog(strerror(errno));
  }else{
    dlog("Memory attached!");
  }
  return shm;
}

/* Write the messenge on shered memory
    @param msg_send, the struct of the message to send */
void shm_write_process(Msg msg_send){
  int shmid = shm_get();
  char* shm = shm_attach(shmid);
  msg_send.type = 0;
  strcpy(shm, msg_send.text);
}

/* Read the message from the shared memory
    @return shm, the messenge readed */
char* shm_read_process(){
  int shmid = shm_get();
  char* shm = shm_attach(shmid);
  return shm;
}
