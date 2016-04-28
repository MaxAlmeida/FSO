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

#define SENADOR   0
#define DEPUTADO  1
#define VEREADOR  2
int N; //numero de parlamentares
int state[N];
Parlamentar parlamentares[N];
sem_t mutex;
int status[3];
void parlamentar(int i){
  srand(time(NULL));
  usleep(rand() % 1000);
  enter_room();
  vote();
  quit_room();
}
/*
void take_fork(int i){
  down(&mutex);  // area crítica
  state[i] == HUNGRY; // READY
  test(i);
  up(&mutex);  // sai da area critica
  down(&s[i]);  // block entrada da sala
}

void put_fork(int i){
  down(&mutex);
  state[i] = THINKING;
  test(LEFT);
  test(RIGHT);
  up(&mutex);
}

void test(int i){ // numero do filosofo
  if(status[i]==HUNGER && status[LEFT]!= EATING && status[RIGHT]!= EATING){
    status[i] = EATING;// VOTANDO
    up(&s[i]);
  }
}
*/

void meditate(){
  srand(time(NULL));
  int time = rand() % 1000;
  usleep(time);
}

void enter_room(int i){
  sem_wait(&mutex);
  state[i] = READY
  test(i);
  sem_post(&mutex);
  sem_wait(&parlamentares[i]);
}

void quit_room(int i){
  sem_wait(&mutex);
  state[i] = DONE;
  test(i);

}

void vote(){
  usleep(50);
}

typedef struct sems{
  sem_t senador;
  sem_t vereador;
  sem_t deputado;
}Sems;

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

  
  key_t key = KEY;
  int shm_id;
  shm_id = shm_create (key);
  sems *attr;
  attr = (sems) shmat (shm_id, NULL, 0);
  sem_init(attr->senador, 1, 1);	   

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
