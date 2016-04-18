/**
  Requisitos

  [] Senadores não podem entrar na sala simultaneamente
  [] Ninguem pode entrar na sala qnd um senador estiver votando
  [] Até 5 deputados podem entrar na sala juntos
  [] Sem limites para vereadores
  [] Vereadores e depultados podem entrar juntos

  [] Utilizar memória compartilhada e semáforo
  [] Informar o numero de senadores, depultados e vereadores (opção por flag para rnd)
  [] Cria processos para cada parlamentar (senador, deputado e vereador)
  [] Antes de entrar na sala deve meditar por 0~1000
  [] A votação deve ser simulada com 'sleep'
  [] Pai aguarda todos os processos filhos terminarem
  [] Após o termino, o pai deve remover o SHM e semáforos
  [] Logs de acompanhamento das votações
*/

#include <stdio.h>

#define SENADOR   1
#define DEPUTADO  2
#define VEREADOR  3

#define YES   1
#define NO    0

/* Structure */
typedef struct voter{
  int voter_id;
  int type;
  int wating_time;
} Voter;

typedef struct imp{
  int yes;
  int no;
}Imp;

/* Definitions of the mathods */
void voting(Voter voters[]);
void enter_room(Voter* voter);
void vote(int v);
Voter new_voter(int id, int type, int wating);

/* Definitions of semaphores */


int main(){
  return 0;
}

void voting(Voter voters[]){
  enter_room();
  
}

void enter_room(Voter* voter){
}

void vote(Imp* imp, int v){
  if(v == YES)
    imp->yes++;
  else
    imp->no++;
}

Voter new_voter(int id, int type, int wating){
  Voter voter;
  voter.voter_id = id;
  voter.type = type;
  voter.wating_time = wating;
  return voter;
}

