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

int main(){

    
    return 0; 
}
