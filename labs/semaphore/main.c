/*
  Requisitos

  [X] Senadores não podem entrar na sala simultaneamente
  [X] Ninguem pode entrar na sala qnd um senador estiver votando
  [X] Até 5 deputados podem entrar na sala juntos
  [X] Sem limites para vereadores
  [X] Vereadores e deputados podem entrar juntos

  [X] Utilizar memória compartilhada e semáforo
  [X] Numero de senadores, deputados e vereadores rand
  [X] Cria processos para cada parlamentar (senador, deputado e vereador)
  [X] Antes de entrar na sala deve meditar por 0~1000
  [X] A votação deve ser simulada com 'sleep'
  [X] Pai aguarda todos os processos filhos terminarem
  [X] Após o termino, o pai deve remover o SHM e semáforos
  [X] Logs de acompanhamento das votações
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include "comunication.h"
#include "voting.h"

#define MAX   10

int main(){
  srand(time(NULL));
  int senadores    = rand() % MAX + 1;
  int deputados    = rand() % MAX + 20;
  int vereadores   = rand() % MAX + 50;

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
  sem_init(&semaphore->deputado,1,0);
  sem_init(&semaphore->vereador,1,0);

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
  sem_destroy(&semaphore->senador);
  sem_destroy(&semaphore->deputado);
  sem_destroy(&semaphore->vereador);
  return 0;
}
