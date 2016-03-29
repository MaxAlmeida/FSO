#include<stdio.h>
#include<signal.h>
#include<unistd.h>

/* Signal treatment */
void no_sig(int signal){
  printf("\n----- No Signal Process -----\n");
  printf("** Blocked Signal : %i **", signal);
}

int main(){

  char c;
  int s;
  
  /* Block all signals from 1 to 34 */
  for(s=1; s<35; s++){
    signal(s,no_sig);
  }
  printf("My pid : %i\n", getpid());
  while(1){
    printf("Processing...\n");
    sleep(1);
  }
}

