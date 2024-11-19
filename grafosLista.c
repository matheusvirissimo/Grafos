// !!! NÃO CONSEGUIMOS IMPLEMENTAR A ÁRVORE GERADORA MÍNIMA EFICIENTEMENTE E ATRIBUI-LA AO CÓDIGO. !!!

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct no {
    int v;
    struct no *prox; // próximo elemento conectado
    int peso;
} NO;

typedef NO *ptr_no;

typedef struct grafo {
    ptr_no *adjacencia;
    int n;
} GRAFO;

typedef GRAFO *ptr_grafo;

// Funções para pilha
typedef struct pilha {
    int *elementos;
    int topo;
    int tamanho;
} PILHA;

PILHA *criarPilha(int tamanho) {
    PILHA *p = malloc(sizeof(PILHA));
    p->elementos = malloc(tamanho * sizeof(int));
    p->topo = -1;
    p->tamanho = tamanho;
    return p;
}

void empilhar(PILHA *p, int v) {
    if (p->topo < p->tamanho - 1) {
        p->elementos[++p->topo] = v;
    }
}

int desempilhar(PILHA *p) {
    if (p->topo >= 0) {
        return p->elementos[p->topo--];
    }
    return -1; // Retorna -1 se a pilha estiver vazia
}

int pilhaVazia(PILHA *p) {
    return p->topo == -1;
}

void destruirPilha(PILHA *p) {
    free(p->elementos);
    free(p);
}

// Funções para fila
typedef struct fila {
    int *elementos;
    int inicio;
    int fim;
    int tamanho;
    int capacidade;
} FILA;

FILA *criarFila(int capacidade) {
    FILA *f = malloc(sizeof(FILA));
    f->elementos = malloc(capacidade * sizeof(int));
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
    f->capacidade = capacidade;
    return f;
}

void enfileirar(FILA *f, int v) {
    if (f->tamanho < f->capacidade) {
        f->elementos[f->fim] = v;
        f->fim = (f->fim + 1) % f->capacidade;
        f->tamanho++;
    }
}

int desenfileirar(FILA *f) {
    if (f->tamanho > 0) {
        int v = f->elementos[f->inicio];
        f->inicio = (f->inicio + 1) % f->capacidade;
        f->tamanho--;
        return v;
    }
    return -1; // Retorna -1 se a fila estiver vazia
}

int filaVazia(FILA *f) {
    return f->tamanho == 0;
}

void destruirFila(FILA *f) {
    free(f->elementos);
    free(f);
}

// Funções para grafo
ptr_grafo criarGrafo(int n) {
    ptr_grafo grafo = malloc(sizeof(GRAFO));
    grafo->n = n;
    grafo->adjacencia = malloc(n * sizeof(ptr_no));
    for (int i = 0; i < n; i++) {
        grafo->adjacencia[i] = NULL;
    }
    return grafo;
}

void liberaLista(ptr_no lista) {
    if (lista != NULL) {
        liberaLista(lista->prox);
        free(lista);
    }
}

void destroiGrafo(ptr_grafo grafo) {
    for (int i = 0; i < grafo->n; i++) {
        liberaLista(grafo->adjacencia[i]);
    }
    free(grafo->adjacencia);
    free(grafo);
}

ptr_no insereLista(ptr_no lista, int v) {
    ptr_no novoElemento = malloc(sizeof(NO));
    novoElemento->prox = lista;
    novoElemento->v = v;
    return novoElemento;
}

void insereAresta(ptr_grafo grafo, int u, int v, int peso) {
    ptr_no novoElementoU = malloc(sizeof(NO));
    novoElementoU->v = v;
    novoElementoU->peso = peso;
    novoElementoU->prox = grafo->adjacencia[u];
    grafo->adjacencia[u] = novoElementoU;

    ptr_no novoElementoV = malloc(sizeof(NO));
    novoElementoV->v = u;
    novoElementoV->peso = peso;
    novoElementoV->prox = grafo->adjacencia[v];
    grafo->adjacencia[v] = novoElementoV;
}

// Funções de busca
int *buscaLarguraFila(ptr_grafo grafo, int s) {
    FILA *fila = criarFila(grafo->n);
    int *pai = malloc(grafo->n * sizeof(int));
    int *visitado = malloc(grafo->n * sizeof(int));
    for (int i = 0; i < grafo->n; i++) {
        pai[i] = -1;
        visitado[i] = 0;
    }

    enfileirar(fila, s);
    pai[s] = s;
    visitado[s] = 1;

    while (!filaVazia(fila)) {
        int v = desenfileirar(fila);
        ptr_no noAux;
        for (noAux = grafo->adjacencia[v]; noAux != NULL; noAux = noAux->prox) {
            if (!visitado[noAux->v]) {
                visitado[noAux->v] = 1;
                pai[noAux->v] = v;
                enfileirar(fila, noAux->v);
            }
        }
    }

    destruirFila(fila);
    free(visitado);
    return pai;
}

int *buscaProfundidadePilha(ptr_grafo grafo, int s) {
    PILHA *pilha = criarPilha(grafo->n);
    int *pai = malloc(grafo->n * sizeof(int));
    int *visitado = malloc(grafo->n * sizeof(int));
    for (int i = 0; i < grafo->n; i++) {
        pai[i] = -1;
        visitado[i] = 0;
    }

    empilhar(pilha, s);
    pai[s] = s;

    while (!pilhaVazia(pilha)) {
        int v = desempilhar(pilha);
        if (!visitado[v]) {
            visitado[v] = 1;
            ptr_no noAux;
            for (noAux = grafo->adjacencia[v]; noAux != NULL; noAux = noAux->prox) {
                if (!visitado[noAux->v]) {
                    empilhar(pilha, noAux->v);
                    pai[noAux->v] = v;
                }
            }
        }
    }

    destruirPilha(pilha);
    free(visitado);
    return pai;
}

void imprimirCaminho(int v, int *pai) {
    if (pai[v] != v) {
        imprimirCaminho(pai[v], pai);
    }
    printf("%d ", v);
}


//-----------------------DJIKSTRA--------------------------------------

typedef struct item {
    int prioridade;
    int vertice;
} ITEM;

typedef struct filaprioridade {
    ITEM *v;
    int *indice;
    int n, tamanho;
} FILAPRIORIDADE;

typedef FILAPRIORIDADE *ptr_fp;

// Funções de Fila de Prioridade
int prioridade(ptr_fp fila, int vertice) {
    int i = fila->indice[vertice];
    return (i != -1) ? fila->v[i].prioridade : INT_MAX;
}


ptr_fp criarFilaPrioridade(int tamanho) {
    ptr_fp fila = malloc(sizeof(FILAPRIORIDADE));
    fila->v = malloc(tamanho * sizeof(ITEM));
    fila->indice = malloc(tamanho * sizeof(int));
    fila->n = tamanho;
    fila->tamanho = 0;
    for (int i = 0; i < tamanho; i++)
        fila->indice[i] = -1;
    return fila;
}

void insereFilaPrioridade(ptr_fp fila, int vertice, int prioridade) {
    fila->v[fila->tamanho].vertice = vertice;
    fila->v[fila->tamanho].prioridade = prioridade;
    fila->indice[vertice] = fila->tamanho;
    fila->tamanho++;
}

void diminuirPrioridade(ptr_fp fila, int vertice, int novaPrioridade) {
    int i = fila->indice[vertice];
    fila->v[i].prioridade = novaPrioridade;
    while (i > 0 && fila->v[(i - 1) / 2].prioridade > fila->v[i].prioridade) {
        ITEM temp = fila->v[i];
        fila->v[i] = fila->v[(i - 1) / 2];
        fila->v[(i - 1) / 2] = temp;
        fila->indice[fila->v[i].vertice] = i;
        fila->indice[fila->v[(i - 1) / 2].vertice] = (i - 1) / 2;
        i = (i - 1) / 2;
    }
}

int filaPrioridadeVazia(ptr_fp fila) {
    return fila->tamanho == 0;
}

int extraiMinimo(ptr_fp fila) {
    if (filaPrioridadeVazia(fila)) return -1;

    int minVertice = fila->v[0].vertice;
    fila->v[0] = fila->v[fila->tamanho - 1];
    fila->indice[fila->v[0].vertice] = 0;
    fila->tamanho--;

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

// Implementação de Dijkstra
int *dijkstra(ptr_grafo grafo, int s) {
    int *pai = malloc(grafo->n * sizeof(int));
    ptr_fp filap = criarFilaPrioridade(grafo->n);
    for (int v = 0; v < grafo->n; v++) {
        pai[v] = -1;
        insereFilaPrioridade(filap, v, INT_MAX);
    }

    pai[s] = s;
    diminuirPrioridade(filap, s, 0);

    while (!filaPrioridadeVazia(filap)) {
        int v = extraiMinimo(filap);
        for (ptr_no noAux = grafo->adjacencia[v]; noAux != NULL; noAux = noAux->prox) {
            int novaDist = prioridade(filap, v) + noAux->peso;
            if (novaDist < prioridade(filap, noAux->v)) {
                diminuirPrioridade(filap, noAux->v, novaDist);
                pai[noAux->v] = v;
            }
        }
    }
    return pai;
}



int main() {
    int menu, n, u, v, s, peso;
    ptr_grafo grafo = NULL;

    printf("*** GRAFOS PARA LISTA DE ADJACENCIA ***\n\n");

    while (1) {
        printf("\n1. Criar grafo\n");
        printf("2. Inserir aresta\n");
        printf("3. Busca em largura\n");
        printf("4. Busca em profundidade\n");
        printf("5. Dijkstra\n");
        printf("6. Sair\n");
        printf("Digite sua opcao: ");
        scanf("%d", &menu);

        switch (menu) {
            case 1:
                printf("Digite o número de vértices: ");
                scanf("%d", &n);
                grafo = criarGrafo(n);
                printf("Grafo com %d vértices criado.\n", n);
                break;

            case 2:
                if (grafo == NULL) {
                    printf("Crie o grafo primeiro.\n");
                    break;
                }
                printf("Digite o vértice de origem, destino e peso da aresta (u v peso): ");
                scanf("%d %d %d", &u, &v, &peso);
                insereAresta(grafo, u, v, peso);
                printf("Aresta (%d, %d) com peso %d inserida.\n", u, v, peso);
                break;

            case 3:
                if (grafo == NULL) {
                    printf("Crie o grafo primeiro.\n");
                    break;
                }
                printf("Digite o vértice inicial para a busca em largura: ");
                scanf("%d", &s);
                int *paiLargura = buscaLarguraFila(grafo, s);
                printf("Caminho em largura a partir do vértice %d:\n", s);
                for (int i = 0; i < grafo->n; i++) {
                    if (paiLargura[i] != -1) {
                        imprimirCaminho(i, paiLargura);
                        printf("\n");
                    }
                }
                free(paiLargura);
                break;

            case 4:
                if (grafo == NULL) {
                    printf("Crie o grafo primeiro.\n");
                    break;
                }
                printf("Digite o vértice inicial para a busca em profundidade: ");
                scanf("%d", &s);
                int *paiProfundidade = buscaProfundidadePilha(grafo, s);
                printf("Caminho em profundidade a partir do vértice %d:\n", s);
                for (int i = 0; i < grafo->n; i++) {
                    if (paiProfundidade[i] != -1) {
                        imprimirCaminho(i, paiProfundidade);
                        printf("\n");
                    }
                }
                free(paiProfundidade);
                break;

            case 5:
                if (grafo == NULL) {
                    printf("Crie o grafo primeiro.\n");
                    break;
                }
                printf("Digite o vértice inicial para executar o Dijkstra: ");
                scanf("%d", &s);
                int *paiDijkstra = dijkstra(grafo, s);
                printf("Menores caminhos a partir do vértice %d:\n", s);
                for (int i = 0; i < grafo->n; i++) {
                    if (paiDijkstra[i] != -1) {
                        printf("Caminho até %d: ", i);
                        imprimirCaminho(i, paiDijkstra);
                        printf("\n");
                    }
                }
                free(paiDijkstra);
                break;

            case 6:
                printf("Finalizando.\n");
                if (grafo != NULL) destroiGrafo(grafo);
                return 0;

            default:
                printf("Opção inválida.\n");
                break;
        }
    }
    return 0;
}
