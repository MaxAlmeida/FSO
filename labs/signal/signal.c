#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int a;
/* Signal treatment */
void reset_a(int signal){
  a=0;
}

void nope(int signal){
  printf("\nNope!\n");
}

int main(){
  signal(10,reset_a);
  signal(2,nope);
  pid_t pid = getpid();
  printf("Meu pid eh %i\n",pid);
  while(1){
    printf("Valor de a = %i\n", a);
    a++;
    sleep(3);
  } // fim-while
} // fim-main

