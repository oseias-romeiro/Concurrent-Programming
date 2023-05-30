/*
 * Problema do barbeiro dorminhoco.
 */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CLIENTES 50
#define N_CADEIRAS 5

sem_t sem_cadeiras_de_espera;
sem_t sem_cadeira_cortando;
sem_t sem_cabelo_cortado;
sem_t sem_cadeira_cliente;

void *f_barbeiro(void *v)
{
    while (1)
    {
        // Esperando um cliente
        sem_wait(&sem_cadeira_cliente);
        
        //Cortando o cabelo do cliente
        sleep(1);
        printf("Barbeiro cortou o cabelo de um cliente.\n");
        
        sem_post(&sem_cabelo_cortado); // libera o cliente
    }
    pthread_exit(0);
}

void *f_cliente(void *v)
{
    int id = *(int *)v;
    sleep(id % 3);
    if (sem_trywait(&sem_cadeiras_de_espera) == 0)
    {
        printf("Cliente %d na espera\n", id);
        //... pegar/sentar a cadeira do barbeiro
        sem_wait(&sem_cadeira_cortando);
        printf("Cliente %d sendo tendido\n", id);

        sem_post(&sem_cadeira_cliente); // acorda o barbeiro para cortar seu cabelo
        sem_post(&sem_cadeiras_de_espera); // libera a sua cadeira de espera
        sem_wait(&sem_cabelo_cortado); // aguarda o cabelo ficar na régua
        sem_post(&sem_cadeira_cortando); // libera a cadeira do barbeiro
        printf("Cliente %d saiu\n", id);
    }
    else
    {
        //barbearia cheia
        printf("Cliente %d não coseguiu entrar, barbearia lotada!\n", id);
    }
    pthread_exit(0);
}

int main()
{
    pthread_t thr_clientes[N_CLIENTES], thr_barbeiro;
    int i, id[N_CLIENTES];

    sem_init(&sem_cadeiras_de_espera, 0, N_CADEIRAS);
    sem_init(&sem_cadeira_cortando, 0, 1);
    sem_init(&sem_cadeira_cliente, 0, 0);
    sem_init(&sem_cabelo_cortado, 0, 0);

    for (i = 0; i < N_CLIENTES; i++)
    {
        id[i] = i;
        pthread_create(&thr_clientes[i], NULL, f_cliente, (void *)&id[i]);
    }

    pthread_create(&thr_barbeiro, NULL, f_barbeiro, NULL);

    for (i = 0; i < N_CLIENTES; i++)
        pthread_join(thr_clientes[i], NULL);

    /* Barbeiro assassinado */
    return 0;
}
