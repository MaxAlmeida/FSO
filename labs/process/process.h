bool SHM_SIDE_SND = false;

/* The task executed by Parent */
void parent(int qid_snd, int qid_rcv){
  dlog(":: Parent Exec ::");

  pthread_t th_dequeue;
  pthread_t th_enqueue;
  pthread_create(&th_dequeue, NULL, *dequeue_msg, (void *)qid_rcv);
  pthread_create(&th_enqueue, NULL, *enqueue_msg, (void *)qid_snd);

  pthread_join(th_enqueue,NULL);
  pthread_join(th_dequeue,NULL);
  int status;
  wait(&status); // Wait for child to end
}

/* The task executed by Child */
void child(int qid_snd, int qid_rcv){
  dlog(":: Child Exec ::"); 
  // Check the type of IPC
  if(EN_TCP == true){
    dlog(":: TCP Connection ::");
    // Compress qids
    qIDs *qids = malloc(sizeof(*qids));
    qids->snd = qid_snd;
    qids->rcv = qid_rcv;
    if(SERVER_SIDE){
      pthread_t th_server;
      pthread_create(&th_server, NULL, *server, qids);
      pthread_join(th_server, NULL);
    }else{
      pthread_t th_client;
      pthread_create(&th_client, NULL, *client, qids);
      pthread_join(th_client, NULL);
    }
  }else{
    dlog(":: SHM Connection ::");
    int shmid = shm_create();
    set_checker();
    if(SHM_SIDE_SND){
      pthread_t th_shm_snd;
      pthread_create(&th_shm_snd, NULL, *shm_sender, (void *)qid_snd);
      pthread_join(th_shm_snd, NULL);
    }else{
      pthread_t th_shm_rcv;
      pthread_create(&th_shm_rcv, NULL, *shm_reciever, (void *)qid_rcv);
      pthread_join(th_shm_rcv, NULL);
    }
  }
}
