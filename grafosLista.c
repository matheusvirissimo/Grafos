#include <stdio.h>
#include <stdlib.h>

typedef struct no{
    int v; 
    int peso; // djisktra
    struct no *prox; // próximo elemento conectado
} NO;

typedef NO *ptr_no;

typedef struct grafo{
    ptr_no *adjacencia;
    int n;
} GRAFO;

typedef GRAFO *ptr_grafo;

// Heap binário

typedef struct item{
    int prioridade; 
    int vertice;
} ITEM;

typedef struct filaprioridade{
    ITEM *v;
    int *indice;
    int n, tamanho;
} FILAPRIORIDADE;

typedef FILAPRIORIDADE * ptr_fp;

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

ptr_no insereLista(ptr_no lista, int v){
    ptr_no novoElemento = malloc(sizeof(NO));

    novoElemento->prox = lista;
    novoElemento->v= v;

    return novoElemento;
}

void insereListaRecursivo(ptr_grafo grafo, int u, int v){
    grafo->adjacencia[v] = insereLista(grafo->adjacencia[v], u);
    grafo->adjacencia[u] = insereLista(grafo->adjacencia[u], v);
}

ptr_no removeElemento(ptr_no lista, int chave){
    ptr_no prox;

    if(lista == NULL){ // não possui elemento
        return NULL;
    }
    
    if(lista->v == chave){ // se o elemento procurado for igual a chave
        prox = lista->prox; // prox recebe o próximo elemento a lista
        free(lista); // remove o elemento atual 
        return prox; // retorna o novo elemenot
    }else{
        lista->prox = removeElemento(lista->prox, chave); // vai recursivamente pro próximo elemento
        return lista;
    }
}

void removeAresta(ptr_grafo grafo, int u, int v){
    grafo->adjacencia[u] = removeElemento(grafo->adjacencia[u], v);
    grafo->adjacencia[v] = removeElemento(grafo->adjacencia[v], u);
}

int possuiAresta(ptr_grafo grafo, int u, int v){
    ptr_no noAux;

    for(noAux = grafo->adjacencia[u]; noAux != NULL; noAux = noAux->prox){
        if(noAux->v == v){
            return 1; 
        }
    }

    return 0;

}

void imprimirAresta(ptr_grafo grafo){
    ptr_no noAux;

    for(int u = 0; u < grafo->n; u++){
        for(noAux = grafo->adjacencia[u]; grafo != NULL; noAux = noAux->prox){
            printf("{%d, %d}\n", u, noAux->v);
        }
    }
}

// Componente conexa
void visitaRecursiva(ptr_grafo grafo, int *componente, int comp, int v){
    ptr_no noAux;
    componente[v] = comp;

    for(noAux = grafo->adjacencia[v]; comp != NULL; noAux = noAux->prox){
        if(componente[noAux->v] == -1){
            visitaRecursiva(grafo, componente, comp, noAux->v);
        }
    }
}

int * encontraComponente(ptr_grafo grafo){
    int *componente = malloc(grafo->n * sizeof(int));
    int c = 0;
    int s;

    for(s = 0; s < grafo->n; s++){
        componente[s] = -1;
    }

    for(s = 0; s < grafo->n; s++){
        if(componente[s] == -1){
            visitaRecursiva(grafo, componente, c, s);
            c++;
        }
    }
    
    return componente;
}

// 1° Busca em profundidade
void buscaProfundidade(ptr_grafo grafo, int *pai, int atual, int v){
    ptr_no noAux;

    pai[v] = atual;

    for(noAux = grafo->adjacencia[v]; noAux != NULL; noAux = noAux->prox){
        if(pai[noAux->v] == -1){ // não foi visitado
            buscaProfundidade(grafo, pai, v, noAux->v);
        }
    }
}

int * encontraCaminho(ptr_grafo grafo, int ini){
    int *pai = malloc(grafo->n * sizeof(int));

    for(int i = 0; i < grafo->n; i++){
        pai[i] = -1;
    }

    buscaProfundidade(grafo, pai, ini, ini);
    return pai;

}

void imprimeCaminho(int v, int *pai){
    if(pai[v] != v){
        imprimeCaminhoReverso(pai[v], pai);
    }
    printf("%d", v);
}

void imprimeCaminhoReverso(int v, int *pai){
    printf("%d", v);
    if(pai[v] != v){
        imprimeCaminhoReverso(pai[v], pai);
    }
}

// fazer busca com pilha

//2° busca em LARGURA (fila)

// Ordenação topológica
void visitaRecursivaTopologica(ptr_grafo grafo, int *visitado, int v){
    ptr_no noAux;
    visitado[v] = 1;

    for(noAux = grafo->adjacencia[v]; noAux != NULL; noAux = noAux->prox){
        if(!visitado[noAux->v]){ // visitado[noAux->v] != null
            visitaRecursivaTopologica(grafo, visitado, noAux->v);
        }
    }

    printf("%d ", v);
}

void ordenacaoTopologica(ptr_grafo grafo){
    int *visitado = malloc(grafo->n * sizeof(int));

    for(int i = 0; i < grafo->n; i++){
        visitado[i] = 0;
    }

    for(int i = 0; i < grafo->n; i++){
        if(!visitado[i]){
            visitaRecursivaTopologica(grafo, visitado, i);
        }
    }

    free(visitado);
    printf("\n");
}

int main(){
    return 0;
}