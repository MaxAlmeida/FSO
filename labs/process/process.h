void child_a(int qid){
  key_t key = 5678;
  Msg msg_rcv;
  strcpy(msg_rcv.text, "lll123\t");
  shm_write_process(msg_rcv);
  do{
    char* check = shm_read_process();
    printf("--%s--\n",check);
    if(strcmp(check,"lll123")){  
      read_msg(&msg_rcv,qid);
      shm_write_process(msg_rcv);
    }
  }while(strcmp(msg_rcv.text,"EXIT"));
}

void parent_a(int qid){
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

void child_b(int qid){
  key_t key = 5678; 
  printf("Cheguei no filho B com esse qid %d\n\n",qid);
  Msg msg_send;
  int status;
  msg_send.type = 0;
  char* shm = shm_read_process();
  printf("Filho B - 'Lido da memória essa mensagem: %s'\n",shm);

  Msg msg_check;
  msg_check.type = 0;
  strcpy(msg_check.text,"lll123");
  shm_write_process(msg_check);

  strcpy(msg_send.text,shm);
  send_msg(&msg_send,qid);
  remove_queue(qid);
}

void parent_b(int qid){
//  Msg msg_rcv;
//  read_msg(&msg_rcv,qid);
//  printf("Pai B - 'Recebi essa mensagem do meu filho: %s'\n",msg_rcv.text); 
}
