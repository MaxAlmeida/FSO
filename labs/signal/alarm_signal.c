#include<stdio.h>
#include<signal.h>
#include<unistd.h>

/* Signal treatment */
void raise_msg(int signal){
  printf("\nAborting for timeout!\n");
  raise(15);
}
void timeout(int signal_p){
  printf("\nTimeout! Waiting for new input in 4 seconds\n");
  // Redefining the signal SIGALRM for another function
  signal(SIGALRM, raise_msg);
  alarm(4); // 4 seconds schedule
}

int main(){
  int a;
  signal(SIGALRM, timeout);

  alarm(7); // 7 seconds schedule
  printf("%i", SIGALRM);
  printf("My PID: %i\n");
  printf("Input number: ");
  scanf("%i",&a);
  alarm(0); // cancel schedule
  printf("Thank you!\n");
  return(0);
} // fim-main

