/*
  Requisitos

  [] Senadores não podem entrar na sala simultaneamente
  [] Ninguem pode entrar na sala qnd um senador estiver votando
  [] Até 5 deputados podem entrar na sala juntos
  [] Sem limites para vereadores
  [] Vereadores e deputados podem entrar juntos

  [] Utilizar memória compartilhada e semáforo
  [X] Numero de senadores, deputados e vereadores rand
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
#include "comunication.h"
#include "voting.h"

#define MAX   5
#define TOTAL senadores+deputados+vereadores

int main(){
  srand(time(NULL));
  int senadores    = rand() % MAX + 1;
  int deputados    =0;//= rand() % MAX + 1;
  int vereadores   =0;//= rand() % MAX + 1;

  int pid;    // forks
  int i,j;    // counter
  int status; // waiting
  int number; // parlamentar number
  int type;   // parlamentar type
  
  // Init semaphore with SHM 
  printf("Init Semaphore\n");
  Sems *semaphore;
  shm_id = shm_create (semaphore);
  semaphore = shm_attach(shm_id);
  sem_init(&semaphore->mutex,1,1);
  sem_init(&semaphore->senador,1,0);

  semaphore->qnt_s = 0;
  semaphore->qnt_d = 0;
  semaphore->qnt_v = 0;
  
  // Creating everyone
  int total;
  int p_num = 0;
  for(i=1;i<=3;i++){
    switch(i){
      case SENADOR:   total = senadores;   break;
      case DEPUTADO:  total = deputados;   break;
      case VEREADOR:  total = vereadores;  break;}

    for(j=1;j<=total;j++){
      p_num++;
      pid = fork();
      if(pid==0){
        switch(i){
          case SENADOR:   type = SENADOR;   break;
          case DEPUTADO:  type = DEPUTADO;  break;
          case VEREADOR:  type = VEREADOR;  break;}
        sem_init(&semaphore->parlamentar[p_num-1],1,1);
        number = p_num;
        break;
      }
    }
    if(pid==0) break;
  }
  // Child 
  if(pid==0){
    voting_process(number,type);
  }

  // Waiting for all child to end
  total = senadores + deputados + vereadores; 
  for(i=0; i<total;i++){
    wait(&status);}

  // Destroing semaphore
  sem_destroy(&semaphore->mutex);
  return 0;
}
