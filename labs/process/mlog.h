#include <stdio.h>
#include <stdbool.h>

bool EN_LOG=false;

void dlog(char* msg){
  if(EN_LOG)
    printf("DEBUG: %s\n",msg);
}

void dilog(char* msg, int arg){
  if(EN_LOG)
    printf("DEBUG: %s : %d\n",msg,arg);
}

void dmlog(char* msg, char* arg){
  if(EN_LOG)
    printf("DEBUG: %s : %s\n",msg,arg);
}

void elog(char* msg){
  printf("ERROR: %s\n",msg);
}

void mlog(char* msg){
  if(EN_LOG)
    printf("MSG: %s\n",msg);
}

