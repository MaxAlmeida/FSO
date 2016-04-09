#define CODE "889299"
#define KEY 5678

void parent_a(int qid){
  dlog(":: Parent A EXEC ::");
  int status;
  Msg msg_send;

  // Loop to read input from user
  do{
    msg_send.type = 0;
    strcpy(msg_send.text, user_input());
    send_msg(&msg_send,qid);
  } while(strcmp(msg_send.text,"EXIT"));
  wait(&status);
  remove_queue(qid);
}

void server(){
  init_server();
}

void client(){
  init_client();
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
      dlog("If check code child b");
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

void child_a(int qid){
  if(EN_TCP == true){
    server();
  }else{
    shmem_sender(qid);
  }
}

void parent_b(int qid){
  dlog(":: Parent B EXEC ::");
  int status;
  Msg msg_rcv;
  do{
    read_msg(&msg_rcv,qid);
    printf("[msg]> %s\n", msg_rcv.text);
  }while(strcmp(msg_rcv.text,"EXIT"));
  wait(&status);
}

void child_b(int qid){
  if(EN_TCP == true){
    client();
  }else{
    shmem_rcv(qid);
  }
}
