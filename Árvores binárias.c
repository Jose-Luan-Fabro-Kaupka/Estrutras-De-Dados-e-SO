#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct palavra{
	char *string;
	int *quantidade;
	//Tamanho do vetor quantidade
	int tamanho;
}palavra;

typedef struct no{
	int indice;
	palavra palavra1;
	struct no *esquerda;
	struct no *direita;
}no;

// Definição da estrutura do nó da lista
typedef struct Nolista {
	char *string;
	int index;
	int quantidade;
	struct Nolista *ant;
	struct Nolista *prox;
} Nolista;


//Função para alocar memória para a estrutura palavra
palavra *criaPalavra(char *string) {
	palavra *p = malloc(sizeof(palavra));
	p->string = strdup(string);
	p->quantidade = calloc(1, sizeof(int));
	p->tamanho = 0;
	return p;
}

//Função para alocar memória para o nó
no *criaNo(int indice, char *string) {
	no *n = malloc(sizeof(no));
	n->indice = indice;
	n->palavra1 = *criaPalavra(string);
	n->esquerda = NULL;
	n->direita = NULL;
	return n;
}

//Função para inserir um nó na árvore
void insere(no **raiz, int indice, char *string) {
	if (*raiz == NULL) {
		*raiz = criaNo(indice, string);
	} else {
		int comparacao = strcmp((*raiz)->palavra1.string, string);
		if (comparacao == 0) {
			// A palavra já existe na árvore, ignorar inserção
			return;
		} else if (comparacao > 0) {
			insere(&(*raiz)->esquerda, indice, string);
		} else {
			insere(&(*raiz)->direita, indice, string);
		}
	}
}


//Função para escrever no arquivo de saída
void printArvore(FILE *arq, no *no1) {
	if (no1 != NULL) {
		printArvore(arq, no1->esquerda);
		fprintf(arq, "%s", no1->palavra1.string);
		for (int i = 0; i < no1->palavra1.tamanho; ++i) {
			if (i == 0 && no1->palavra1.tamanho != 0) {
				fprintf(arq, ", ");
			}

			if (i != 0) {
				fprintf(arq, "%d", no1->palavra1.quantidade[i]);

				if (i < no1->palavra1.tamanho - 1) {
					fprintf(arq, ", ");
				}
			}
		}
		fprintf(arq, "\n");
		printArvore(arq, no1->direita);
	}
}

// Função para criar um novo nó
Nolista *criarNolista(char *str, int index) {
    Nolista *novoNolista = (Nolista *)malloc(sizeof(Nolista));
    novoNolista->string = strdup(str);
	novoNolista->index = index;
	novoNolista->quantidade = 0;
    novoNolista->ant= NULL;
    novoNolista->prox = NULL;
    return novoNolista;
}

// Função para inserir um nó no início da lista
void insertNolista(Nolista **raiz, Nolista *novoNolista) {
	if (*raiz == NULL) {
		*raiz = novoNolista;
	} else {
		Nolista *atual = *raiz;
		while (atual->prox != NULL) {
			atual = atual->prox;
		}
		atual->prox = novoNolista;
		novoNolista->ant= atual;
	}
}

// Função para desalocar a lista da memória
void desalocarLista(Nolista **raiz) {
    Nolista *atual = *raiz;
    Nolista *prox;

    while (atual != NULL) {
        prox = atual->prox;
        free(atual->string);
        free(atual);
        atual = prox;
    }

    *raiz = NULL;
}

//Função para ler o arquivo e retornar as palavras em um vetor de strings
Nolista *lerArquivo(FILE *arq){
	char linha[1000];
	Nolista *retorno;
	char *token;
	int i = 0;

	while(fgets(linha, 1000, arq) != NULL) {
		token = strtok(linha, " ");
		while(token != NULL){
			if(i == 0){
				retorno = criarNolista(token, i);
				i++;
			}
			Nolista *no1 = criarNolista(token, i);
			insertNolista(&retorno, no1);
			i++;
			token = strtok(NULL, " ");
		}
	}

	return retorno;
}


// Função para adicionar o número de vezes que uma palavra apareceu em cada página
void incrementar(no **raiz, char *string, int chave, int valor) {
	if (*raiz == NULL) {
		return;
	}
	if (!strcmp((*raiz)->palavra1.string, string)) {
		if (valor >= (*raiz)->palavra1.tamanho) {
			(*raiz)->palavra1.tamanho = valor + 1;
			(*raiz)->palavra1.quantidade = realloc((*raiz)->palavra1.quantidade, (*raiz)->palavra1.tamanho * sizeof(int));
		}
		(*raiz)->palavra1.quantidade[valor]++;
	}
	if (chave < (*raiz)->indice) {
		incrementar(&((*raiz)->esquerda), string, chave, valor);
	} else {
		incrementar(&((*raiz)->direita), string, chave, valor);
	}
}


//Função para ler os termos que serão buscados no arquivo
no* lertermos(char* termos) {
	no* arvore = NULL;
	char** palavras = NULL;

	char* token = strtok(termos, ",");
	int n = 0;

	// Divisão das palavras pelas vírgulas
	while (token != NULL) {
		palavras = (char**)realloc(palavras, sizeof(char*) * (n + 1));
		palavras[n] = strdup(token);
		token = strtok(NULL, ",");
		n++;
	}

	for (int i = 0; i < n; ++i) {
		if (i == 0) {
			int tamanho = strlen(palavras[i]) - 8;
			char aux[tamanho];
			int k = 0;
			for (int j = 0; j < strlen(palavras[i]); ++j) {
				if (j >= 8) {
					aux[k] = palavras[i][j];
					k++;
				}
			}
			arvore = criaNo(i, aux);
		} else if (i == (n - 1)) {
			int tamanho = strlen(palavras[i]) - 2;
			char* aux = (char*)malloc((tamanho + 1) * sizeof(char));
			int k = 0;
			for (int j = 0; j < tamanho; ++j) {
				aux[k] = palavras[i][j];
				k++;
			}
			aux[k] = '\0';
			insere(&arvore, i, aux);
			free(aux);
		} else {
			insere(&arvore, i, palavras[i]);
		}
	}

	free(palavras);

	return arvore;
}

void comparar(no **arvore, Nolista *lista) {
	Nolista *aux = lista;
	int pg = 0;
	char *substring = "<page:";

	while (aux != NULL) {
		if (strstr(aux->string, substring) != NULL) {
			sscanf(aux->string, "%*[^0-9]%d", &pg);
		} else {
			// Procurar a palavra na árvore
			no *no1 = *arvore;
			while (no1 != NULL) {
				int comparacao = strcmp(no1->palavra1.string, aux->string);
				if (comparacao == 0) {
					// Palavra encontrada na árvore, incrementar contagem
					incrementar(arvore, no1->palavra1.string, no1->indice, pg);
					break;
				} else if (comparacao > 0) {
					no1 = no1->esquerda;
				} else {
					no1 = no1->direita;
				}
			}
		}
		aux = aux->prox;
	}
}

void desalocarArvore(no *raiz) {
	if (raiz == NULL) {
		return;
	}

	desalocarArvore(raiz->esquerda);
	desalocarArvore(raiz->direita);

	free(raiz->palavra1.string);
	free(raiz->palavra1.quantidade);
	free(raiz);
}


int main(int argc, const char *argv[]) {
	FILE *arq;
	arq = fopen(argv[1], "r");

	if(arq == NULL){
		printf("Erro ao abrir o arquivo %s", argv[1]);
		return 1;
	}

	Nolista *ret = lerArquivo(arq);

	no *no1 = lertermos(ret->string);

	comparar(&no1, ret);

	FILE *output;
	output = fopen(argv[2], "w");

	if(output == NULL){
		printf("Erro ao abrir o arquivo %s", argv[2]);
		return 1;
	}

	printArvore(output, no1);

	fclose(arq);
	fclose(output);
	desalocarLista(&ret);
	desalocarArvore(no1);
	return 0;
}
