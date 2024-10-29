typedef struct grafo{
    int **adjacente; 
    int n; // quantidade de vértices
} Grafo;

typedef Grafo *ptr_grafo; // == ponteiro_grafo nova_variavel. É só pra facilitar que se coloca assim. Vai-se utilizar bastante ponteiro

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
    free(grafo->n); // free nos vértices
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

int main(){

    
    return 0; 
}
