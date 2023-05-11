#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAXCANIBAIS 20

pthread_mutex_t lock_porcoes = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond_cozinheiro = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_canibais = PTHREAD_COND_INITIALIZER;

int porcoes = 0;

void *canibal(void*meuid);
void *cozinheiro(int m);


void main(argc, argv)
int argc;
char *argv[];
{
  int erro;
  int i, n, m;
  int *id;

  pthread_t tid[MAXCANIBAIS];

  if(argc != 3){
    printf("erro na chamada do programa: jantar <#canibais> <#comida>\n");
    exit(1);
  }
  
  n = atoi (argv[1]); // número de canibais
  m = atoi (argv[2]); // quantidade de porções que o cozinheiro consegue preparar por vez
  printf("numero de canibais: %d -- quantidade de comida: %d\n", n, m);

  if(n > MAXCANIBAIS){
    printf("o numero de canibais e' maior que o maximo permitido: %d\n", MAXCANIBAIS);
    exit(1);
  }
  
  for (i = 0; i < n; i++)  {
    id = (int *) malloc(sizeof(int));
    *id = i;
    erro = pthread_create(&tid[i], NULL, canibal, (void *) (id));

    if(erro){
      printf("erro na criacao do thread %d\n", i);
      exit(1);
    }
  }

  cozinheiro(m);
}

void * canibal (void* pi){
  int id = *(int *)(pi);
  while(1) {
    sleep(1);
    pthread_mutex_lock(&lock_porcoes);

    // canibal dorme caso o caldeirão esteja vazio
    while(porcoes == 0)
    {
      printf("canibal %d dormindo\n", id);
      pthread_cond_wait(&cond_canibais, &lock_porcoes);
    }
    
    // canibal acorda e come uma porção
    printf("%d: vou comer a porcao 1/%d\n", id, porcoes);
    porcoes--;
    sleep(2);

    // caso ele pegue a ultima porção ele acorda o conzinheiro
    if (porcoes == 0)
    {
      pthread_cond_signal(&cond_cozinheiro);
    }

    pthread_mutex_unlock(&lock_porcoes);
  }
  pthread_exit(0);
}

void *cozinheiro (int m){
 
  while(1){
    sleep(1);
    pthread_mutex_lock(&lock_porcoes);

    // dorme enquanto ainda tem porções no caldeirão
    while (porcoes > 0)
    {
      pthread_cond_wait(&cond_cozinheiro, &lock_porcoes);
    }
    
    // conzinheiro preparando a refeição
    printf("cozinheiro preparando %d porcoes\n", m);
    sleep(5);
    porcoes = m;

    // consinheiro acorda os canibais que dormem, esperando a refeição ficar pronta
    pthread_cond_broadcast(&cond_canibais);

    pthread_mutex_unlock(&lock_porcoes);
  }
  pthread_exit(0);
}
