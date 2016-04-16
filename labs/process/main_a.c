
#include "pthread.h" 
#include "mlog.h"
#include "comunication.h"
#include "tcp.h"
#include "threads.h"
#include "process.h"

int main(int argc, char* argv[]){
  // Setting up Flags
  flags(argc,argv); 
  // Setting up Queue
  int qid_send = create_queue();
  int qid_receive = create_queue();
  // Getting the user name
  set_name();

  int pid = fork();
  if(pid > 0){ // Parent
    parent(qid_send, qid_receive);
  }else if(pid == 0){ // Child
    SHM_SIDE_SND = true;
    SERVER_SIDE = true;
    child(qid_send, qid_receive);
  }else if(pid < 0){ // Fail
    printf("Failed to create process!\n", pid);
    exit(-1);
  }

  // Remove Queue
  remove_queue(qid_send);
  remove_queue(qid_receive);
} // fim-main
