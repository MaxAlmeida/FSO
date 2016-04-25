/*
  Requisitos

  [] Senadores não podem entrar na sala simultaneamente
  [] Ninguem pode entrar na sala qnd um senador estiver votando
  [] Até 5 deputados podem entrar na sala juntos
  [] Sem limites para vereadores
  [] Vereadores e depultados podem entrar juntos

  [] Utilizar memória compartilhada e semáforo
  [X] Numero de senadores, depultados e vereadores rand
  [X] Cria processos para cada parlamentar (senador, deputado e vereador)
  [] Antes de entrar na sala deve meditar por 0~1000
  [] A votação deve ser simulada com 'sleep'
  [] Pai aguarda todos os processos filhos terminarem
  [] Após o termino, o pai deve remover o SHM e semáforos
  [] Logs de acompanhamento das votações
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

#define SENADOR   1
#define DEPUTADO  2
#define VEREADOR  3


void meditate(){
  srand(time(NULL));
  int time = rand() % 1000;
  usleep(time);
}

void enter_room(){
}

void vote(){
  usleep(50);
}

void voting_process(){
//  sem_wait(); // Enter Critical Section
  meditate();
  enter_room();
  vote();
//  sem_post(); // Exit Critical Section
}

typedef struct parlamentar{
  int type;
}Parlamentar;

int main(){
  srand(time(NULL));
  int senador   = rand() % 50;
  int deputado  = rand() % 50;
  int vereador  = rand() % 50;
  int i; // counter
  int status; // waiting status

  // Creating the 'Senadores'
  for(i=0;i<senador;i++){
    pid = fork();
    if(pid < 0){printf("Failed to fork!\n");}
    else if(pid==0){ // Child
      // Starting voting
      Parlamentar new_parlamentar;
      new_parlamentar.type = SENADOR;
    }
  }

  // Creating the 'Deputados'
  for(i=0;i<deputado;i++){
    pid = fork();
    if(pid < 0){printf("Failed to fork!\n");}
    else if(pid==0){ // Child
      // Starting voting
      Parlamentar new_parlamentar;
      new_parlamentar.type = DEPUTADO;
    }
  }

  // Creating the 'Vereadores'
  for(i=0;i<vereador;i++){
    pid = fork();
    if(pid < 0){printf("Failed to fork!\n");}
    else if(pid==0){ // Child
      // Starting voting
      Parlamentar new_parlamentar;
      new_parlamentar.type = VEREADOR;
    }
  }

  wait(&status);
  return 0;
}
