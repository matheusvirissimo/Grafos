#include <stdio.h>
#include <stdlib.h>

typedef struct no{
    int v; 
    struct no *prox; // próximo elemento conectado
} NO;

typedef NO *ptr_no;

typedef struct grafo{
    ptr_no *adjacente;
    int n;
} GRAFO;

typedef GRAFO *ptr_grafo;