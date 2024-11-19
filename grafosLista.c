#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

//-------------------------------------------------

/* int * buscaProfundidadePilha(p_grafo g, int s) {
    int v, w;
    int *pai = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    p_pilha p = criarPilha();
    for(v = 0; v < g->n; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }
    empilha(p, s);
    pai[s] = s;
    while(!pilhavazia(p)) {
        v = desempilha(p);
        visitado[v] = 1;
        for(w = 0; w< g->n; w++) {
            if(g->adj[v][w] && !visitado[w]) {
                pai[w] = v;
                empilha(p, w);
            }
        }
    }
    destroipilha(p);
    free(visitado);
    return pai;
}

int * buscaLarguraFila(p_grafo g, int s) {
    int v, w;
    int *pai = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    p_fila f = criarFila();
    for(v = 0; v < g->n; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }
    enfilera(f, s);
    pai[s] = s;
    visitado[v]=1;
    while(!filavazia(f)) {
        v = desenfilera(f);
        for(w = 0; w < g->n; w++) {
            pai[w] = v;
            visitado[w] = 1;
            enfilera(f, w);
        }
    }
    destroifila(f);
    free(visitado);
    return(pai);
}

*/

// códigos de fila com prioridade - LLM

// Função para criar a fila de prioridade
ptr_fp criarFilaPrioridade(int tamanho) {
    ptr_fp fila = malloc(sizeof(FILAPRIORIDADE));
    fila->v = malloc(tamanho * sizeof(ITEM));
    fila->indice = malloc(tamanho * sizeof(int));
    fila->n = tamanho;
    fila->tamanho = 0;
    for (int i = 0; i < tamanho; i++)
        fila->indice[i] = -1; // Inicialmente, nenhum vértice está na fila
    return fila;
}

// Função para inserir um elemento na fila de prioridade
void insereFilaPrioridade(ptr_fp fila, int vertice, int prioridade) {
    fila->v[fila->tamanho].vertice = vertice;
    fila->v[fila->tamanho].prioridade = prioridade;
    fila->indice[vertice] = fila->tamanho;
    fila->tamanho++;
}

// Função para diminuir a prioridade de um elemento
void diminuirPrioridade(ptr_fp fila, int vertice, int novaPrioridade) {
    int i = fila->indice[vertice];
    fila->v[i].prioridade = novaPrioridade;

    // Realoca o item para cima no heap para restaurar a ordem
    while (i > 0 && fila->v[(i - 1) / 2].prioridade > fila->v[i].prioridade) {
        ITEM temp = fila->v[i];
        fila->v[i] = fila->v[(i - 1) / 2];
        fila->v[(i - 1) / 2] = temp;
        fila->indice[fila->v[i].vertice] = i;
        fila->indice[fila->v[(i - 1) / 2].vertice] = (i - 1) / 2;
        i = (i - 1) / 2;
    }
}

// Função para verificar se a fila está vazia
int filaPrioridadeVazia(ptr_fp fila) {
    return fila->tamanho == 0;
}

// Função para obter a prioridade de um elemento
int prioridade(ptr_fp fila, int vertice) {
    int i = fila->indice[vertice];
    return (i != -1) ? fila->v[i].prioridade : INT_MAX;
}

// Função para extrair o elemento com menor prioridade
int extraiMinimo(ptr_fp fila) {
    if (filaPrioridadeVazia(fila)) return -1;

    int minVertice = fila->v[0].vertice;
    fila->v[0] = fila->v[fila->tamanho - 1];
    fila->indice[fila->v[0].vertice] = 0;
    fila->tamanho--;

    // Reorganiza o heap
    int i = 0;
    while (2 * i + 1 < fila->tamanho) {
        int menorFilho = 2 * i + 1;
        if (menorFilho + 1 < fila->tamanho && fila->v[menorFilho + 1].prioridade < fila->v[menorFilho].prioridade)
            menorFilho++;

        if (fila->v[i].prioridade <= fila->v[menorFilho].prioridade)
            break;

        ITEM temp = fila->v[i];
        fila->v[i] = fila->v[menorFilho];
        fila->v[menorFilho] = temp;
        fila->indice[fila->v[i].vertice] = i;
        fila->indice[fila->v[menorFilho].vertice] = menorFilho;

        i = menorFilho;
    }
    return minVertice;
}

// 3° Dijkstra
int * dijkstra(ptr_grafo grafo, int s){
    int *pai = malloc(grafo->n * sizeof(int));
    int v;

    ptr_no noAux;
    ptr_fp filap = criarFilaPrioridade(grafo->n); 
    for(v = 0; v < grafo->n; v++){
        pai[v] = -1;
        insereFilaPrioridade(filap, v, INT_MAX); 
    }

    pai[s] = s;
    diminuirPrioridade(filap, s, 0); 
    while(!filaPrioridadeVazia(filap)){ 
        v = extraiMinimo(filap); 
        if(prioridade(filap, v) != INT_MAX){ 
            for(noAux = grafo->adjacencia[v]; noAux != NULL; noAux = noAux->prox){
                if(prioridade(filap, v)+noAux->peso < prioridadade(filap, noAux->v)){
                    diminuiPrioridade(filap, noAux->v, (prioridade(filap, v)+noAux->peso));
                    pai[noAux->v] = v;
                }
            }
        }
    }

    return pai;

}

// 4° Árvore Geradora Mínima
// Função para encontrar a Árvore Geradora Mínima usando Prim
int* arvoreGeradoraMinima(ptr_grafo grafo, int s) {
    int *pai = malloc(grafo->n * sizeof(int)); // Armazena o pai de cada nó na MST
    int *custo = malloc(grafo->n * sizeof(int)); // Armazena o peso mínimo para cada nó
    int v;

    ptr_no noAux;
    ptr_fp filap = criarFilaPrioridade(grafo->n);
    for (v = 0; v < grafo->n; v++) {
        pai[v] = -1;
        custo[v] = INT_MAX;
        insereFilaPrioridade(filap, v, INT_MAX);
    }

    pai[s] = s;
    custo[s] = 0;
    diminuirPrioridade(filap, s, 0);

    while (!filaPrioridadeVazia(filap)) {
        v = extraiMinimo(filap);

        for (noAux = grafo->adjacencia[v]; noAux != NULL; noAux = noAux->prox) {
            int u = noAux->v;
            int peso = noAux->peso;

            // Verifica se u ainda está na fila e se podemos diminuir o peso
            if (prioridade(filap, u) > peso) {
                pai[u] = v;
                custo[u] = peso;
                diminuirPrioridade(filap, u, peso);
            }
        }
    }

    free(custo);
    return pai;
}

int main(){
    return 0;
}