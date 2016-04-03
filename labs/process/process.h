
void child_a(int qid){
  key_t key = 5678;
  check_msg(qid);
  if(shm_get(key) < 0 && check_msg(qid) >= 0){
    dlog("Entrando no while");
    Msg msg_rcv;
    read_msg(&msg_rcv,qid);
    printf("Message Received:\n%s\n",msg_rcv.text);
    shm_write_process(msg_rcv);
  }
  dlog("end while"); 
  check_msg(qid);
}

void parent_a(int qid){
  char message[250];
 
  printf("Enter the message: \n");
  while(scanf("%s", message)){
    Msg msg_send;
    msg_send.type = 0;
    strcpy(msg_send.text, message); 
    int status;
   // printf(">>%s\n", msg_send->text);
    send_msg(&msg_send,qid);
    send_msg(&msg_send,qid);
    wait(&status);
  }
    //remove_queue(qid);
}

void child_b(int qid){
  key_t key = 5678; 
  printf("Cheguei no filho B com esse qid %d\n\n",qid);
  Msg msg_send;
  int status;
  msg_send.type = 0;
  char* shm = shm_read_process();
  printf("Filho B - 'Lido da memória essa mensagem: %s'\n",shm);
  strcpy(msg_send.text,shm);
  send_msg(&msg_send,qid);
  wait(&status);
  int shmid = shm_get(key);
  clear_memmory(shm,shmid);
  remove_queue(qid);
}

void parent_b(int qid){
  Msg msg_rcv;
  read_msg(&msg_rcv,qid);
  printf("Pai B - 'Recebi essa mensagem do meu filho: %s'\n",msg_rcv.text); 
}
