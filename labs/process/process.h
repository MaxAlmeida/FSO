#define CODE "8892x99"
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

void child_a(int qid){
  dlog(":: Child A EXEC ::");
  key_t key = KEY;
  Msg msg_rcv;
  
  do{
    char* check = shm_read_process();
    sleep(1);
    if(!strcmp(check,CODE)){
      read_msg(&msg_rcv,qid);
      dmlog("Shm has sender code!", msg_rcv.text);
      shm_write_process(msg_rcv);
    }
  }while(strcmp(msg_rcv.text,"EXIT"));
}

void parent_b(int qid){
  Msg msg_rcv;
  read_msg(&msg_rcv,qid);
  dmlog(":: Parent B :: Recive the message", msg_rcv.text);
}

void child_b(int qid){
  dlog(":: Child B EXEC ::");
  key_t key = KEY; 
  Msg msg_send;
  int status;
  msg_send.type = 0;
  char* shm = shm_read_process();
  dmlog(":: Child B :: Mansagem",shm);

  Msg msg_check;
  msg_check.type = 0;
  strcpy(msg_check.text,CODE);
  shm_write_process(msg_check);

  strcpy(msg_send.text,shm);
  send_msg(&msg_send,qid);
  remove_queue(qid);
}
