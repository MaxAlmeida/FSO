
#define SENADOR   1
#define DEPUTADO  2
#define VEREADOR  3

/* GLOBAL */
int shm_id;

/* FOR DEBUG ONLY */
void print_qnt(){
  Sems* semaphore = shm_read_process(shm_id);
  printf("SENADORES: %d DEPUTADOS: %d VEREADORES: %d\n", semaphore->qnt_s, semaphore->qnt_d, semaphore->qnt_v);
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
int ck_room(int number,int type){
  int enter = 0;
  Sems* semaphore = shm_read_process(shm_id);
  switch(type){
    case SENADOR: // Check for SENADOR
      if(semaphore->qnt_s == 0 &&
        semaphore->qnt_d == 0 &&
        semaphore->qnt_v == 0){// Empty room
        enter = 1;
      }
      break;
    case DEPUTADO: // Check for DEPUTADO
      if(semaphore->qnt_s == 0 &&
        semaphore->qnt_d < 5){
        enter = 1;
      }
      break;
    case VEREADOR:
      if(semaphore->qnt_s ==0){
        enter = 1;
      }
      break;
  }
  return enter;
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
  int get_inside = 0;
  Sems* semaphore = shm_read_process(shm_id);

  do{
    sem_wait(&semaphore->mutex); // Enter Critical Section
    get_inside = ck_room(number,type);
    if(get_inside==1){
      print_qnt();
      print_type(number,type);
      printf("entering the room\n");
      switch(type){
        case SENADOR:   semaphore->qnt_s++; break;
        case DEPUTADO:  semaphore->qnt_d++; break;
        case VEREADOR:  semaphore->qnt_v++; break;
      }
      print_qnt();
      sem_post(&semaphore->mutex); // Exit Critical Section
    }else{
      sem_post(&semaphore->mutex); // Exit Critical Section
      switch(type){
        case SENADOR:   sem_wait(&semaphore->senador);  break;
        case DEPUTADO:  sem_wait(&semaphore->deputado); break;
        case VEREADOR:  sem_wait(&semaphore->vereador); break;
      }
      
    }
  }while(get_inside==0);

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
    case SENADOR:   semaphore->qnt_s--; sem_post(&semaphore->senador);  break;
    case DEPUTADO:  semaphore->qnt_d--; sem_post(&semaphore->deputado); break;
    case VEREADOR:  semaphore->qnt_v--; sem_post(&semaphore->vereador); break;
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

