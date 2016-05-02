
#define SENADOR   1
#define DEPUTADO  2
#define VEREADOR  3

/* GLOBAL */
int shm_id;

/* FOR DEBUG ONLY */
void print_qnt(){
  Sems* semaphore = shm_read_process(shm_id);
  printf("SENADORES: %d\n", semaphore->qnt_s);
}

/* Print the type of the senador */
void print_type(int number, int type){
  switch(type){
    case SENADOR:   printf("Senador s%d ", number);  break;
    case DEPUTADO:  printf("Deputado d%d ", number); break;
    case VEREADOR:  printf("Vereador v%d ", number); break;
  }
}

/* Checking the room */
void ck_room(int type){
  Sems* semaphore = shm_read_process(shm_id);
  print_qnt();
  if(type == SENADOR){// Checking for senador
    if(semaphore->qnt_s == 0 &&
      semaphore->qnt_d == 0 &&
      semaphore->qnt_v == 0){// Empty room
      printf("EMPTY ROOM!\n");
    }else{// Not empty room
      printf("NOT EMPTY ROOM!\n");
      sem_wait(&semaphore->);
    }
  }
}

/* Parlamentar meditating  */
void meditate(int number, int type){
  print_type(number,type);
  printf("meditating...\n");
  srand(time(NULL));
  int time = rand() % 1000;
  usleep(time);
}

/*IMPORTANT*/
/* Parlamentar entering the room*/
void enter_room(int number, int type){
  Sems* semaphore = shm_read_process(shm_id);
  sem_wait(&semaphore->mutex); // Enter Critical Section
  ck_room(type);
  print_type(number,type);
  printf("entering the room\n");
  switch(type){
    case SENADOR:   semaphore->qnt_s++; break;
    case DEPUTADO:  semaphore->qnt_d++; break;
    case VEREADOR:  semaphore->qnt_v++; break;
  }
  sem_post(&semaphore->mutex); // Exit Critical Section
}

/* Parlamentar voting... */
void vote(int number, int type){
  print_type(number,type);
  printf("voting...\n");
  usleep(50);
}

/*IMPORTANT*/
/* Parlamentar leaving the room */
void leaving_room(int number, int type){
  Sems* semaphore = shm_read_process(shm_id);
  sem_wait(&semaphore->mutex); // Enter Critical Section
  print_type(number,type);
  printf("leaving...\n");
  switch(type){
    case SENADOR:   semaphore->qnt_s--; break;
    case DEPUTADO:  semaphore->qnt_d--; break;
    case VEREADOR:  semaphore->qnt_v--; break;
  }
  sem_post(&semaphore->mutex); // Exit Critical Section
}

/* Parlamentar voting process */
void voting_process(int number, int type){ 
  meditate(number,type);
  enter_room(number,type);
  vote(number,type);
  leaving_room(number,type);
}

