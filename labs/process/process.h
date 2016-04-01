
void child_a(int qid){
  Msg msg_rcv;
  read_msg(&msg_rcv,qid);
  printf("Message Received:\n%s\n",msg_rcv.text);

  shm_write_process(msg_rcv);
}

void parent_a(int qid){
  Msg msg_send;
  int status;
  msg_send.type = 0;
  strcpy(msg_send.text, "Testando Mensagem");
  send_msg(&msg_send,qid);
  wait(&status);
  remove_queue(qid);
}

void child_b(int qid){
  printf("Cheguei no filho B com esse qid %d\n\n",qid);
  Msg msg_send;
  int status;
  msg_send.type = 0;
  char* shm = shm_read_process();
  printf("Filho B - 'Lido da memória essa mensagem: %s'\n",shm);
  strcpy(msg_send.text,shm);
  send_msg(&msg_send,qid);
  wait(&status);
  remove_queue(qid);
}

void parent_b(int qid){
  Msg msg_rcv;
  read_msg(&msg_rcv,qid);
  printf("Pai B - 'Recebi essa mensagem do meu filho: %s'\n",msg_rcv.text); 
}