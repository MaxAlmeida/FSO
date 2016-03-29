#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void main(void){
  //pid_t pid;
  int pid = fork();
  if(pid < 0){
    printf("Doesn't have resource!\n");
  } else {
    if(pid > 0){
      printf("Father - %d\n", pid);
      sleep(30);
      execl("/bin/ls","ls","-la","fork.c",NULL) ;
    } else {
      printf("Child - %d\n", pid);
      char *arg[] = {"", "-al", NULL};
      char *arg2[] = {"-al",NULL};
      execve("/bin/ls",arg,NULL);
    }
  }
}
