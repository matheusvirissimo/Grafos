// !!! NÃO CONSEGUIMOS IMPLEMENTAR A ÁRVORE GERADORA MÍNIMA EFICIENTEMENTE E ATRIBUI-LA AO CÓDIGO. !!!

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct grafo{
    int **adjacente; 
    int n; // quantidade de vértices
} Grafo;

typedef Grafo *ptr_grafo;

typedef struct pilha {
    int *dados;
    int topo;
    int capacidade;
} Pilha;

typedef Pilha *ptr_pilha;

typedef struct fila {
    int *dados;
    int inicio;
    int fim;
    int tamanho;
    int capacidade;
} Fila;

typedef Fila *ptr_fila;

// Funções para pilha
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

// Funções para fila
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

// Funções para grafo
ptr_grafo criar_grafo(int n) {
    ptr_grafo novoGrafo = malloc(sizeof(Grafo));
    novoGrafo->n = n;
    novoGrafo->adjacente = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        novoGrafo->adjacente[i] = malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            novoGrafo->adjacente[i][j] = INT_MAX; // Sem aresta inicialmente
        }
    }
    return novoGrafo;
}

void destruir_grafo(ptr_grafo grafo) {
    for (int i = 0; i < grafo->n; i++) {
        free(grafo->adjacente[i]);
    }
    free(grafo->adjacente);
    free(grafo);
}

// Insere aresta no grafo
void insere_aresta(ptr_grafo grafo, int u, int v, int peso) {
    grafo->adjacente[u][v] = peso;
    grafo->adjacente[v][u] = peso; // Grafo não-direcionado
}

// Funções para busca em largura
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
            if (g->adjacente[v][w] != INT_MAX && !visitado[w]) {
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

// Funções para busca em profundidade
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
                if (g->adjacente[v][w] != INT_MAX && !visitado[w]) {
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

// Funções para Dijkstra
int menor_distancia(int *distancia, int *visitado, int n) {
    int min = INT_MAX, indice_min = -1;
    for (int i = 0; i < n; i++) {
        if (!visitado[i] && distancia[i] < min) {
            min = distancia[i];
            indice_min = i;
        }
    }
    return indice_min;
}

int *dijkstra(ptr_grafo grafo, int origem) {
    int *distancia = malloc(grafo->n * sizeof(int));
    int *pai = malloc(grafo->n * sizeof(int));
    int *visitado = malloc(grafo->n * sizeof(int));

    for (int i = 0; i < grafo->n; i++) {
        distancia[i] = INT_MAX;
        pai[i] = -1;
        visitado[i] = 0;
    }

    distancia[origem] = 0;
    pai[origem] = origem;

    for (int i = 0; i < grafo->n - 1; i++) {
        int u = menor_distancia(distancia, visitado, grafo->n);
        if (u == -1) break;
        visitado[u] = 1;

        for (int v = 0; v < grafo->n; v++) {
            if (!visitado[v] && grafo->adjacente[u][v] != INT_MAX &&
                distancia[u] + grafo->adjacente[u][v] < distancia[v]) {
                distancia[v] = distancia[u] + grafo->adjacente[u][v];
                pai[v] = u;
            }
        }
    }

    free(visitado);
    return pai;
}

// Imprimir caminho
void imprimir_caminho(int v, int *pai) {
    if (pai[v] != v) {
        imprimir_caminho(pai[v], pai);
    }
    printf("%d ", v);
}

// Main
int main() {
    int menu, n, u, v, s, peso;
    ptr_grafo grafo = NULL;

    printf("*** GRAFOS PARA MATRIZ DE ADJACENCIA ***\n\n");

    while (1) {
        printf("\n1. Criar grafo\n");
        printf("2. Inserir aresta\n");
        printf("3. Dijkstra\n");
        printf("4. Busca em Largura\n");
        printf("5. Busca em Profundidade\n");
        printf("6. Sair\n");
        printf("Digite sua opcao: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                printf("Digite o número de vértices: ");
                scanf("%d", &n);
                grafo = criar_grafo(n);
                printf("Grafo com %d vértices criado.\n", n);
                break;

            case 2:
                if (grafo == NULL) {
                    printf("Crie o grafo primeiro.\n");
                    break;
                }
                printf("Digite o vértice de origem, destino e peso da aresta (u v peso): ");
                scanf("%d %d %d", &u, &v, &peso);
                insere_aresta(grafo, u, v, peso);
                printf("Aresta (%d, %d) com peso %d inserida.\n", u, v, peso);
                break;

            case 3:
                if (grafo == NULL) {
                    printf("Crie o grafo primeiro.\n");
                    break;
                }
                printf("Digite o vértice de origem para Dijkstra: ");
                scanf("%d", &s);
                int *paiDijkstra = dijkstra(grafo, s);
                printf("Caminhos mínimos a partir do vértice %d:\n", s);
                for (int i = 0; i < grafo->n; i++) {
                    if (paiDijkstra[i] != -1) {
                        printf("Caminho até %d: ", i);
                        imprimir_caminho(i, paiDijkstra);
                        printf("\n");
                    }
                }
                free(paiDijkstra);
                break;

            case 4:
                if (grafo == NULL) {
                    printf("Crie o grafo primeiro.\n");
                    break;
                }
                printf("Digite o vértice de origem para Busca em Largura: ");
                scanf("%d", &s);
                int *paiLargura = busca_largura_fila(grafo, s);
                printf("Árvore de busca em largura a partir do vértice %d:\n", s);
                for (int i = 0; i < grafo->n; i++) {
                    if (paiLargura[i] != -1) {
                        printf("Caminho até %d: ", i);
                        imprimir_caminho(i, paiLargura);
                        printf("\n");
                    }
                }
                free(paiLargura);
                break;

            case 5:
                if (grafo == NULL) {
                    printf("Crie o grafo primeiro.\n");
                    break;
                }
                printf("Digite o vértice de origem para Busca em Profundidade: ");
                scanf("%d", &s);
                int *paiProfundidade = busca_profundidade_pilha(grafo, s);
                printf("Árvore de busca em profundidade a partir do vértice %d:\n", s);
                for (int i = 0; i < grafo->n; i++) {
                    if (paiProfundidade[i] != -1) {
                        printf("Caminho até %d: ", i);
                        imprimir_caminho(i, paiProfundidade);
                        printf("\n");
                    }
                }
                free(paiProfundidade);
                break;

            case 6:
                printf("Finalizando.\n");
                if (grafo != NULL) destruir_grafo(grafo);
                return 0;

            default:
                printf("Opção inválida.\n");
                break;
        }
    }
    return 0;
}
