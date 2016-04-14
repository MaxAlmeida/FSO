#define CODE "889299"
#define KEY 5678

void parent_a(int qid_send, int qid_receive){
  dlog(":: Parent A EXEC ::");
  int status;
  Msg msg_send;
  pthread_t rThread;

    char temp[250];
  int ret = pthread_create(&rThread, NULL, printMessage, (void *)qid_receive);
  // Loop to read input from user
  do{
    
    msg_send.type = 0;
    strcpy(msg_send.text, user_input());
    send_msg(&msg_send,qid_send);
    strcpy(temp,msg_send.text);
    temp[strcspn(temp, "\n")] = '\0';
   // printf("APaguei a quebra de linha\n\n");			
  } while(strcmp(temp,"EXIT"));
  wait(&status);
  remove_queue(qid_send);
}
/* Qid_send from queue messsages sended by father A */
/* Qid_receive from child B by trhead receive Message */
void server(int qid_send, int qid_receive){
  int client = init_server();
  InfoMsg *info;
  info = malloc(sizeof(*info));
  char temp[250]; 
  info->qid = qid_receive;
  info->client = client;
  Msg msg_send;
  pthread_t rThread;
  int ret = pthread_create(&rThread, NULL, receiveMessage, info);
  do{
   
    read_msg(&msg_send,qid_send);
    write(client, msg_send.text,sizeof(msg_send.text));
    strcpy(temp,msg_send.text);
    temp[strcspn(temp, "\n")] = '\0';			
  }while(strcmp(temp,"EXIT"));
  close(client);
}

void client(int qid_send, int qid_receive){
  int socket = init_client();
  InfoMsg *info;
  info = malloc(sizeof(*info));
  char temp[250];
  info->qid = qid_send;
  info->client = socket;
  Msg msg_rcv;
  pthread_t rThread;
  int ret = pthread_create(&rThread, NULL, sendMessage, info);
  char msg[250];
  do{
    //recv(socket, msg, strlen(msg),0);
    read(socket, msg_rcv.text, sizeof(msg_rcv.text)); 
    dmlog("Client socket rcv", msg);
    send_msg(&msg_rcv,qid_receive);
    sleep(1);
    strcpy(temp,msg_rcv.text);
    temp[strcspn(temp, "\n")] = '\0';			
  }while(strcmp(temp,"EXIT"));
}

void shmem_sender(int qid){
  dlog(":: Child A EXEC ::");
  key_t key = KEY;
  Msg msg_rcv;

  do{
    //Shared memmory process
    char* check = shm_read_process();
    sleep(1);
    if(!strcmp(check,CODE)){
      read_msg(&msg_rcv,qid);
      shm_write_process(msg_rcv);
      dmlog("Shm has sender code!", msg_rcv.text);
    }
  }while(strcmp(msg_rcv.text,"EXIT"));
}

void shmem_rcv(int qid){
  dlog(":: Child B EXEC ::");
  key_t key = KEY;
  int status;
  Msg msg_send;
  Msg check;

  check.type = 0;
  strcpy(check.text,CODE);

  char* shm = shm_read_process();
  do{
    sleep(1);
    if(strcmp(shm,CODE)){
      // Shmem
      shm = shm_read_process();
      // Send to stack
      strcpy(msg_send.text,shm);
      send_msg(&msg_send,qid);
      dmlog("SHM ",shm);

      shm_write_process(check);
    }
  }while(strcmp(msg_send.text,"EXIT"));
  remove_queue(qid);
}

void child_a(int qid_send, int qid_receive){
  if(EN_TCP == true){
    server(qid_send, qid_receive);
  }else{
    shmem_sender(qid_send);
  }
}

void parent_b(int qid_send, int qid_receive){
  dlog(":: Parent B EXEC ::");
  char temp[250];
  int status;
  pthread_t rThread;
  Msg msg_rcv;
  int ret = pthread_create(&rThread, NULL, readMessage, (void *)qid_send);
  do{
    read_msg(&msg_rcv,qid_receive);
    strcpy(temp,msg_rcv.text);
    temp[strcspn(temp, "\n")] = '\0';			
    printf("<Other:> %s", msg_rcv.text);
  }while(strcmp(temp,"EXIT"));
  wait(&status);
}

void child_b(int qid_send, int qid_receive){
  if(EN_TCP == true){
    client(qid_send, qid_receive);
  }else{
    shmem_rcv(qid_receive);
  }
}
