#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define N 5

void *handle(void *arg) {
  int id = *((int *)arg);
  printf("novo pthread, id=%d\n", id);
  pthread_exit(0);
}

int main() {
  pthread_t arr[N];
  printf("Meu id %lu\n", pthread_self());

  int *id;
  for (int i = 0; i < N; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;

    pthread_create(&arr[i], NULL, handle, (id));
  }
  for (int i=0; i<N; i++){
    pthread_join(arr[i], NULL);
  }
  printf("Fim.\n");
  return 0;
}
