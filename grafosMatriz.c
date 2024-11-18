#include <stdlib.h>
#include <stdio.h>

typedef struct grafo{
    int **adjacente; 
    int n; // quantidade de vértices
} Grafo;

typedef Grafo *ptr_grafo; // == ponteiro_grafo nova_variavel. É só pra facilitar que se coloca assim. Vai-se utilizar bastante ponteiro

// Estrutura da pilha
typedef struct pilha {
    int *dados;
    int topo;
    int capacidade;
} Pilha;

typedef Pilha *ptr_pilha;

// Estrutura da fila
typedef struct fila {
    int *dados;
    int inicio;
    int fim;
    int tamanho;
    int capacidade;
} Fila;

typedef Fila *ptr_fila;


// Funções da pilha
ptr_pilha criar_pilha(int capacidade) {
    ptr_pilha p = malloc(sizeof(Pilha));
    p->dados = malloc(capacidade * sizeof(int));
    p->topo = -1;
    p->capacidade = capacidade;
    return p;
}

int pilha_vazia(ptr_pilha p) {
    return p->topo == -1;
}

void empilha(ptr_pilha p, int valor) {
    p->dados[++p->topo] = valor;
}

int desempilha(ptr_pilha p) {
    return p->dados[p->topo--];
}

void destruir_pilha(ptr_pilha p) {
    free(p->dados);
    free(p);
}

// Funções da fila
ptr_fila criar_fila(int capacidade) {
    ptr_fila f = malloc(sizeof(Fila));
    f->dados = malloc(capacidade * sizeof(int));
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    f->capacidade = capacidade;
    return f;
}

int fila_vazia(ptr_fila f) {
    return f->tamanho == 0;
}

void enfileira(ptr_fila f, int valor) {
    f->dados[f->fim] = valor;
    f->fim = (f->fim + 1) % f->capacidade;
    f->tamanho++;
}

int desenfileira(ptr_fila f) {
    int valor = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % f->capacidade;
    f->tamanho--;
    return valor;
}

void destruir_fila(ptr_fila f) {
    free(f->dados);
    free(f);
}


ptr_grafo criar_grafo(int n){
    int i, j;
    ptr_grafo novoGrafo = malloc(sizeof(Grafo)); // aloca memória para o novo grafo que será construido. Não é necessário fazer casting

    novoGrafo->n = n; // vai receber a quantidade de vértices existentes
    novoGrafo->adjacente = malloc(n * sizeof(int *)); // a lista de adjacência vai alocar memória para cada vértice 

    for(i = 0; i < n; i++){
        novoGrafo->adjacente[i] = malloc(n * sizeof(int)); // aloca memória para cada elemento
    }

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            novoGrafo->adjacente[i][j] = 0; // para cada elemento da matriz de adjacência, sera alocado, inicialmente, 0.
        }
    }

    return novoGrafo;

}

void destruir_grafo(ptr_grafo grafo){
    int i; 
    for(i = 0; i < grafo->n; i++){
        free(grafo->adjacente[i]); // livra cada um dos elementos contido na lista
    }

    free(grafo->adjacente); // dá free na lista em si
    free(grafo); // free nos vértices
}


void insere_aresta(ptr_grafo grafo, int u, int v){
    // na matriz, caso o objeto tenha conexão com o outro, na matriz, isso será dizer que ele é 1
    grafo->adjacente[u][v] = 1; 
    grafo->adjacente[v][u] = 1;

    // não importa a direção
}

void remove_aresta(ptr_grafo grafo, int u, int v){
    // se o objeto não tem conexão, ele é zero. Não se dá free porque 0 indica que não está conectado
    grafo->adjacente[u][v] = 0;
    grafo->adjacente[v][u] = 0;
}

int tem_aresta(ptr_grafo grafo, int u, int v){
    return grafo->adjacente[u][v]; // se retornar 1, tem aresta, do contrário, não tem
}

ptr_grafo le_grafo(){
    int qtdArestas, qtdVertices, u, v;
    ptr_grafo grafo;

    scanf("%d %d", &qtdArestas, &qtdVertices);

    for(int i = 0; i < qtdVertices; i++){
        scanf("%d %d", &u, &v);
        insere_aresta(grafo, u, v);
    }

    return grafo;
}

void imprime_arestas(ptr_grafo grafo){
    int u, v;

    for(u = 0; u < grafo->n; u++){
        for(v = u + 1; v < grafo->n; v++){
            if(grafo->adjacente[u][v]){
                printf("{%d, %d}\n", u, v);
            }
        }
    }
}

int grau(ptr_grafo grafo, int u){
    int grau = 0;

    for(int v = 0; v < grafo->n; v++){
        if(grafo->adjacente[u][v]){ // se houver conexão entre os objetos
            grau++;
        }
    }

    return grau;
}

int mais_popular(ptr_grafo grafo){
    int u, max, grauMaximo, grauAtual;
    max = 0; 
    grauMaximo = grau(grafo, 0);

    for(u = 1; u < grafo->n; u++){
        grauAtual = grau(grafo, u);

        if(grauAtual > grauMaximo){
            grauMaximo = grauAtual;
            max = u;
        }
    }

    return max; // vai retornar a quantidade de vizinhos do mais famoso
}

void imprimir_recomendacoes(ptr_grafo grafo, int u){
    for(int v = 0; v < grafo->n; v++){
        if(grafo->adjacente[u][v]){
            for(int w = 0; w < grafo->n; w++){
                if(grafo->adjacente[v][w] && w != u && !(grafo->adjacente[u][v])){
                    printf("%d\n", w);
                }
            }

        }
    }

}

// 1° Busca de profundidade
int buscaRecursiva(ptr_grafo grafo, int *visitado, int ini, int fim){
    if(ini == fim){
        return 1; // sempre existe caminho para o mesmo valor
    }

    visitado[ini] = 1; // primeiro valor já foi visitado, isso é, ele não é igual ao final

    for(int atual = 0; atual < grafo->n; atual++){
        if(grafo->adjacente[ini][atual] && !visitado[atual]){ // visitado[atual] == 0
            if(buscaRecursiva(grafo, visitado, atual, fim)){
                return 1;
            }
        }
    }

    return 0; // não foi encontrado
}

int existeCaminho(ptr_grafo grafo, int ini, int fim){
    int encontrou;
    int *visitado = malloc(grafo->n * sizeof(int));

    // vamos fazer uma segunda matriz para indicar qual valor já foi visitado ou não
    for(int i = 0; i < grafo->n; i++){
        visitado[i] = 0; // todos os valores da nova matriz, serão, inicialmente 0
    }

    encontrou = buscaRecursiva(grafo, visitado, ini, fim);
    free(visitado);

    return encontrou; // se for igual a 1, encontrou, do contrário, não existe
}



//-------------------------------------------


// Busca em profundidade com pilha
int *busca_profundidade_pilha(ptr_grafo g, int s) {
    int *pai = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    ptr_pilha p = criar_pilha(g->n);

    for (int v = 0; v < g->n; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }

    empilha(p, s);
    pai[s] = s;

    while (!pilha_vazia(p)) {
        int v = desempilha(p);
        if (!visitado[v]) {
            visitado[v] = 1;
            for (int w = 0; w < g->n; w++) {
                if (g->adjacente[v][w] && !visitado[w]) {
                    pai[w] = v;
                    empilha(p, w);
                }
            }
        }
    }

    destruir_pilha(p);
    free(visitado);
    return pai;
}

// Busca em largura com fila
int *busca_largura_fila(ptr_grafo g, int s) {
    int *pai = malloc(g->n * sizeof(int));
    int *visitado = malloc(g->n * sizeof(int));
    ptr_fila f = criar_fila(g->n);

    for (int v = 0; v < g->n; v++) {
        pai[v] = -1;
        visitado[v] = 0;
    }

    enfileira(f, s);
    pai[s] = s;
    visitado[s] = 1;

    while (!fila_vazia(f)) {
        int v = desenfileira(f);
        for (int w = 0; w < g->n; w++) {
            if (g->adjacente[v][w] && !visitado[w]) {
                pai[w] = v;
                enfileira(f, w);
                visitado[w] = 1;
            }
        }
    }

    destruir_fila(f);
    free(visitado);
    return pai;
}


int main(){
    int menu;

    printf("*** GRAFOS PARA MATRIZ DE ADJACENCIA ***\n\n");

    while(1){
        printf("1. Busca em largura\n");
        printf("2. Busca em profundidade\n");
        printf("3. Algoritmo de Djisktra\n");
        printf("4. Arvore Geradora Minima\n");
        printf("5. Sair\n");
        printf("Digite sua opcao: ");
        scanf("%d", &menu);

        switch(menu){
            case 1:
                
                break;
            case 2:
                
                break;
            case 3:
                
                break;
            case 4:
                
                break;
            case 5:
                printf("Finalizando.\n");
                return 0;
                break;
            default:
                break;
        }
    }
    
    return 0; 
}
