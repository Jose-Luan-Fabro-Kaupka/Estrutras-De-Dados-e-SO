#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

// Definição da estrutura da fila
typedef struct fila{
    int BUFFER[BUFFER_SIZE];
    int cont;
    int frente;
    int tras;
}fila;

// Função para inicializar a fila
fila *inicia(){
    fila *fil = (fila*) malloc(sizeof(fila));
    fil->frente = 0;
    fil->tras = 0;
    fil->cont = 0;
    return fil;
}

// Insere um elemento na fila
void por(int i, fila *fil){
    fil->BUFFER[fil->tras] = i;
    fil->tras++;
    fil->cont++;
}

// Remove um elemento da fila
int tirar(fila *fil){
    int aux = fil->BUFFER[fil->frente];
    fil->BUFFER[fil->frente] = 0;
    fil->frente++;
    fil->cont--;
    return aux;
}

fila *fila1;

// Inicializa os semáforos
sem_t mutex;
sem_t vazia;
sem_t cheia;

void *produtor(int arg[]){
    for (int i = 0; i < BUFFER_SIZE; ++i) {

        sem_wait(&cheia); // Espera se a fila estiver cheia
        sem_wait(&mutex); // Bloqueia a fila para outros threads

        por(arg[i], fila1);

        sem_post(&mutex); // Libera a fila
        sem_post(&vazia); // Avisa que há um item na fila
    }
    pthread_exit(NULL);
}

void *consumidor(char args[]){
    int item;
    int soma = 0;

    for (int i = 0; i < BUFFER_SIZE; ++i) {

        sem_wait(&vazia); // Espera se a fila estiver vazia
        sem_wait(&mutex); // Bloqueia a fila para outros threads

        item = tirar(fila1);
        soma += item;

        sem_post(&mutex); // Libera a fila
        sem_post(&cheia); // Avisa que há espaço na fila
    }

    char str[5];

    FILE * arqo = fopen(args, "w");

    if(arqo == NULL){
        printf("Erro ao abrir o arquivo %s", args);
    }

    sprintf(str, "%d", soma);
    strcat(str, "\n");
    fputs(str, arqo);

    fclose(arqo);

    pthread_exit(NULL);
}

int main(int argc, const char *argv[]) {

    setlocale(LC_ALL, "Portuguese");

    FILE *arqi = fopen(argv[1], "r");

    if (argc != 3) {
        printf("Número de parâmetros inválido!");
    }

    if (arqi == NULL) {
        printf("Erro ao abrir o arquivo %s", argv[1]);
    }

    char str[5];

    int v[BUFFER_SIZE];

    int i = 0;
    while (fgets(str, 5, arqi) != NULL) {
        v[i] = atoi(str);
        i++;
    }

    pthread_t prod, cons;
    fila1 = inicia();

    // Inicializa os semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&vazia, 0, 0);
    sem_init(&cheia, 0, BUFFER_SIZE);

    pthread_create(&prod, NULL, (void *(*)(void *)) produtor, v);
    pthread_create(&cons, NULL, (void *(*)(void *)) consumidor, argv[2]);
    pthread_join(prod, NULL);
}
