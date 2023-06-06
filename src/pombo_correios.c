#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "unistd.h"

#define N 10 //numero de usuarios
#define CARTAS 20 //quantidade de cartas na mochila

sem_t pombo_cartas;
sem_t mutex;
sem_t pombo_dormindo;

int contador_cartas = 0;

void * f_usuario(void *arg);
void * f_pombo(void *arg);

int main(int argc, char **argv){
    int i;

    sem_init(&pombo_cartas, 0, CARTAS);
    sem_init(&mutex, 0, 1);
    sem_init(&pombo_dormindo, 0, 0);

    pthread_t usuario[N];
    int *id;
    for(i = 0; i < N; i++){
         id = (int *) malloc(sizeof(int));
          *id = i;
	pthread_create(&(usuario[i]),NULL,f_usuario,  (void *) (id));
    }
     pthread_t pombo;
     id = (int *) malloc(sizeof(int));
     *id = 0;
     pthread_create(&(pombo),NULL,f_pombo, (void*) (id));

     pthread_join(pombo,NULL);
   
}


void * f_pombo(void *arg){
    while(1){
        sleep(1);
        //Inicialmente esta em A, aguardar/dorme a mochila ficar cheia (20 cartas)
        sem_wait(&pombo_dormindo);
        //Leva as cartas para B e volta para A
        printf("pombo entregando cartas\n");
        sleep(5);
        //Acordar os usuarios
        for (int i = 0; i < CARTAS; i++)
            sem_post(&pombo_cartas);
        printf("pombo voltou\n");
    }
}

void * f_usuario(void *arg){
    int id = *((int*) arg);
    while(1){
        sleep(1);
        sem_wait(&mutex);
	    //Escreve uma carta
        printf("usuario %d escrevendo carta\n", id);
        sleep(2);

        //Caso o pombo nao esteja em A ou a mochila estiver cheia, entao dorme	
        sem_wait(&pombo_dormindo);
        contador_cartas++;
        
        //Posta sua carta na mochila do pombo
        printf("usuario %d postando carta\n", id);
        if (contador_cartas >= CARTAS) {
            sem_post(&pombo_dormindo);
        }
        sem_post(&mutex);
    }
}
