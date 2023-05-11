#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PR 5 // número de produtores
#define CN 2 // número de consumidores
#define N 5  // tamanho do buffer

pthread_mutex_t alterna;
pthread_cond_t cond;

int buffer_len = 0;
int buffer[N];

void *produtor(void *pi) {
  int i = *((int *)pi);
  while (1) {
    sleep(1);
    pthread_mutex_lock(&alterna);

    while (buffer_len == N) {
      printf("produtor %d esperando\n", i);
      pthread_cond_wait(&cond, &alterna);
    }

    sleep(5);
    buffer[buffer_len] = rand();
    printf("produtor %d escrevendo, buffer[%d] = %d\n", i, buffer_len,
           buffer[buffer_len]);
    buffer_len++;

    if (buffer_len == 1) {
      pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&alterna);
  }
  pthread_exit(0);
}

void *consumidor(void *pi) {
  int i = *((int *)pi);
  while (1) {
    sleep(1);
    pthread_mutex_lock(&alterna);

    while (buffer_len == 0) {
      printf("consumidor %d esperando\n", i);
      pthread_cond_wait(&cond, &alterna);
    }

    sleep(1);
    printf("consumidor %d lendo, buffer[%d]: %d\n", i, buffer_len - 1,
           buffer[buffer_len - 1]);
    buffer[buffer_len - 1] = NULL;
    buffer_len--;

    if (buffer_len == N - 1) {
      pthread_cond_signal(&cond);
    }

    pthread_mutex_unlock(&alterna);
  }
  pthread_exit(0);
}

int main() {
  pthread_mutex_init(&alterna, NULL);
  pthread_cond_init(&cond, NULL);

  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[PR];

  for (i = 0; i < PR; i++) {
    id = (int *)malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, produtor, (void *)(id));

    if (erro) {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  pthread_t tCid[CN];

  for (i = 0; i < CN; i++) {
    id = (int *)malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tCid[i], NULL, consumidor, (void *)(id));

    if (erro) {
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }
  pthread_join(tid[0], NULL);

  return 0;
}
