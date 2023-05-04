#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#define MAX 100

typedef struct pilha{
    int lis[MAX];
    int topo;
}pilha;

// Função que converte um número decimal para binário
int converte(int dec) {
    int bin = 0;
    int i = 1;
    int resto;

    while (dec != 0){
        resto = dec % 2;
        dec /= 2;
        bin += resto * i;
        i *= 10;
    }

    return bin;
}

// Função que inicializa a pilha
pilha *inicia(){
    pilha *pilha1 = (pilha*) malloc(sizeof(pilha));
    pilha1->topo = 0;
    return pilha1;
}

// Função que verifica se a pilha está cheia
bool cheia(pilha *pilha1){
    return (pilha1->topo == MAX);
}

// Função que verifica se a pilha está vazia
bool vazia(pilha *pilha1){
    return (pilha1->topo == 0);
}

// Função que insere um valor na pilha
bool push(pilha *pilha1, int valor){
    if(cheia(pilha1)){
        return false;
    }

    pilha1->lis[pilha1->topo] = valor;
    pilha1->topo++;

    return true;
}

// Função que remove o valor no topo da pilha
int pop(pilha *pilha1){
    if(vazia(pilha1)){
        return false;
    }

    int aux = pilha1->lis[pilha1->topo - 1];
    pilha1->lis[pilha1->topo - 1] = 0;
    pilha1->topo--;

    return aux;
}

int main(int argc, const char *argv[]) {

    setlocale(LC_ALL, "Portuguese");

    FILE * arqi = fopen(argv[1], "r");

    if(argc != 3){
        printf("Quantidade de parâmetros inválida!");
        return 1;
    }

    if(arqi == NULL){
        printf("O arquivo %s não existe!", argv[1]);
        return 1;
    }

    char str[5];

    pilha *pil = inicia();

    // Lê o arquivo de entrada, converte cada linha para binário e insere na pilha
    while(fgets(str, 5, arqi) != NULL){
        push(pil, converte(atoi(str)));
    }

    if(pil->topo == 0){
        printf("O arquivo %s está vazio!", argv[1]);
        return 1;
    }

    fclose(arqi);

    char str1[5];

    FILE * arqo = fopen(argv[2], "w");

    if(arqo == NULL){
        printf("O arquivo %s não existe!", argv[2]);
        return 1;
    }

    // Percorre a pilha do topo até a base e escreve cada elemento no arquivo de saída.
    for (int i = pil->topo - 1; i >= 0; i--) {
        sprintf(str1, "%d", pop(pil));
        strcat(str1, "\n");
        fputs(str1, arqo);
    }

    fclose(arqo);

    return 0;
}
