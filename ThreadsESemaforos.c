#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define MAX 100
#define BUFFER_SIZE 10

typedef struct fila{
    int BUFFER[MAX];
    int cont;
    int frente;
    int tras;
}fila;

fila *inicia(){
    fila *fil = (fila*) malloc(sizeof(fila));
    fil->frente = 0;
    fil->tras = 0;
    fil->cont = 0;
    return fil;
}

void por(int i, fila *fil){
    fil->BUFFER[fil->tras] = i;
    fil->tras++;
    fil->cont++;
}

int tirar(fila *fil){
    int aux = fil->BUFFER[fil->frente];
    fil->BUFFER[fil->frente] = 0;
    fil->frente++;
    fil->cont--;
    return aux;
}

fila *fila1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t vazia = PTHREAD_COND_INITIALIZER;
pthread_cond_t cheia = PTHREAD_COND_INITIALIZER;

void *produtor(void *args){
    int item;

    for (int i = 0; i < MAX; ++i) {
        item = rand() % 100;
        pthread_mutex_lock(&mutex);
        while (fila1->cont == BUFFER_SIZE){
            pthread_cond_wait(&cheia, &mutex);
        }
        por(item, fila1);
        pthread_cond_signal(&vazia);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *consumidor(void *args){
    int item;
    int soma = 0;

    for (int i = 0; i < MAX; ++i) {
        pthread_mutex_lock(&mutex);
        while (fila1->cont == 0){
            pthread_cond_wait(&vazia, &mutex);
        }
        item = tirar(fila1);
        soma += item;
        pthread_cond_signal(&cheia);
        pthread_mutex_unlock(&mutex);
    }
    printf("Soma: %d", soma);
    pthread_exit(NULL);
}

int main() {
    pthread_t prod, cons;
    fila1 = inicia();
    srand(time(NULL)); // inicializar gerador de números aleatórios
    pthread_create(&prod, NULL, produtor, (void*) fila1);
    pthread_create(&cons, NULL, consumidor, (void*) fila1);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    return 0;
}
