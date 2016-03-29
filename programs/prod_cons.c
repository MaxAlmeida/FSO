#define N 100
int count = 0;

void producer(void){
  int item;
  while(true){
    item = prod_item();
    if(count==N){
      sleep();
    }
    insert_item(item);
    count++;
    if(count==1){
      wakeup(consumer);
    }
  }
}

void consumer(void){
  int item;
  while(true){
    if(count==0){
      sleep();
    }
    item = remove_item();
    count--;
    if(count==N-1){
      wakeup(producer);
    }
    consume_item(item);
  }
}
