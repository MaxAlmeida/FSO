#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int a;
void fail_kill(int sinal){
  a = 0;
}
void main(){
  signal(9,fail_kill);
  printf("Meu pid %i\n",getpid());
  while(1){
    printf("Valor de a = %i\n", a);
    a++;
    sleep(3);
  }
}
