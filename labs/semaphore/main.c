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
typedef struct sems{
  sem_t senador;
  sem_t deputado;
  int senador_only;
}Sems;

#include "comunication.h"
void meditate(int number){
  printf("Eu processo de numero %d pensando aqui na morte da bezerra\n",number);

  srand(time(NULL));
  int time = rand() % 1000;
  sleep(1);
}
void enter_room(int number){
  printf("Processo de numero %d entra na sala\n",number);
}

void vote(int number){
  printf("Processo de numero %d vota neste momento\n",number);
  sleep(1);
}

void voting_process(int number, int shmid){ 
  Sems* semaphore = shm_read_process(shmid);
  meditate(number);
  sem_wait(&semaphore->senador); // Enter Critical Section
  semaphore->senador_only = 1; //only senator in room
  enter_room(number);
  vote(number);
  sem_post(&semaphore->senador); // Exit Critical Section
}

int main(){
  srand(time(NULL));
  int senador   = 5;
  int deputado  = rand() % 50;
  int vereador  = rand() % 50;
  int i; // counter
  int status;
  int pid;
  int number;
  key_t key = 5678;
  int shm_id;
  Sems *semaphore;
  
  //create memmory area
  shm_id = shm_create (semaphore);
 
  //atach semaphore to memmory area
  semaphore = shm_attach(shm_id);

 // shm_write_process(semaphore);
  
  //initiates semaphores 
  sem_init(&semaphore->senador,1,1);

  // Creating the 'Senadores'
  for(i=0;i<senador;i++){
    printf("Fork %d\n",i);
    pid = fork();
    if(pid==0){
      number = i;
      break;
    }
  }
  if(pid==0){
    voting_process(number,shm_id);
  }
  sem_destroy(&semaphore->senador);
  for(int a = 0; a <senador;a++){
   wait(&status);
  }
  return 0;
}
