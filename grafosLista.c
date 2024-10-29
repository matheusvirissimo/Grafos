#include <stdio.h>
#include <stdlib.h>

typedef struct no{
    int v; 
    struct no *prox; // próximo elemento conectado
} NO;

typedef NO *ptr_no;

typedef struct grafo{
    ptr_no *adjacencia;
    int n;
} GRAFO;

typedef GRAFO *ptr_grafo;

ptr_grafo criarGrafo(int n){
    ptr_grafo grafo = malloc(sizeof(GRAFO));

    grafo->n = n;
    grafo->adjacencia = (n * sizeof(ptr_no));

    for(int i = 0; i < n; i++){
        grafo->adjacencia[i] = NULL;
    }

    return grafo;
}

void liberaLista(ptr_no lista){
    if(lista != NULL){
        liberaLista(lista->prox);
        free(lista);
    }
}

void destroiGrafo(ptr_grafo grafo){
    for(int i = 0; i < grafo->n; i++){
        liberaLista(grafo->adjacencia[i]);
    }

    free(grafo->adjacencia);
    free(grafo);
}