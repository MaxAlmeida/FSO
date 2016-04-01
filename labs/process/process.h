
void child(int qid){
  Msg msg_rcv;
  read_msg(&msg_rcv,qid);
  printf("Message Received:\n%s\n",msg_rcv.text);

  shm_process(msg_rcv);
}

void parent(int qid){
  Msg msg_send;
  int status;
  msg_send.type = 0;
  strcpy(msg_send.text, "HELL");

  send_msg(&msg_send,qid);
  wait(&status);
  remove_queue(qid);
}
