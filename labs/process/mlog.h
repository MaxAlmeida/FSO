#include <stdbool.h>
bool EN_LOG=false;

void dlog(char* msg){
  if(EN_LOG)
    printf("BUG: %s\n",msg);
}

void elog(char* msg){
  if(EN_LOG)
    printf("ERROR: %s\n",msg);
}

void mlog(char* msg){
  if(EN_LOG)
    printf("MSG: %s\n",msg);
}

