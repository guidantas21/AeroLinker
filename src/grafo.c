#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "../include/grafo.h"

// Libera toda a memória alocada para o grafo
void liberarGrafo(tGrafo *grafo) {
    // Verifica se a matriz adjacente precisa ser liberada
    if (grafo->arestas != NULL) {
        for (int i = 0; i < grafo->numVertices; i++) {
            // Libera memória de cada vetor da matriz adjacente que não seja nulo
            if (grafo->arestas[i] != NULL) {
                free(grafo->arestas[i]);
                grafo->arestas[i] = NULL;
            }
        }
        // Libera memória da matriz adjacente
        free(grafo->arestas);
        grafo->arestas = NULL;
    }
    // Libera memória do grafo
    free(grafo);
    grafo = NULL;
}

// Cria um novo grafo com o número especificado de vértices
tGrafo *criarGrafo(int numVertices) {
    assert(numVertices > 0); // Verifica se o número de vértices é maior que 0
    
    // Aloca memória para um novo grafo
    tGrafo *novoGrafo = (tGrafo*) malloc(sizeof(tGrafo));

    // Verifica alocação do novo grafo
    if (novoGrafo == NULL) {
        return NULL;
    }

    novoGrafo->numVertices = numVertices;
    // Aloca memória para uma matriz adjacente de n vetores de arestas
    novoGrafo->arestas = (tAresta**) malloc(numVertices * sizeof(tAresta*));

    // Verifica alocação da matriz adjacente
    if (novoGrafo->arestas == NULL) {
        free(novoGrafo);
        novoGrafo = NULL;
        return NULL;
    }

    for (int i = 0; i < numVertices; i++) {
        // Aloca memória para cada vetor de n arestas da matriz 
        novoGrafo->arestas[i] = (tAresta*) malloc(numVertices * sizeof(tAresta));
        // Inicializa a matriz adjacente com distâncias iniciais
        for (int j = 0; j < numVertices; j++) {
            // Se o vertice inicial for o mesmo do final, a distância da aresta entre eles é 0
            if (i == j) {
                novoGrafo->arestas[i][j].distancia = 0; // Distância entre vértices iguais é zero
            } else {
                novoGrafo->arestas[i][j].distancia = -1; // Inicializa com distância -1 (sem aresta)
            }
            // Verifica alocação de cada vetor de arestas da matriz adjacente
            if (novoGrafo->arestas[i] == NULL) {
                liberarGrafo(novoGrafo);
                return NULL;
            }
        }
    }

    return novoGrafo;
}

// Verifica se uma aresta existe entre dois vértices no grafo
bool existeAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino) {
    // Verifica se os vértices de origem e destino da aresta são válidos
    assert(verticeOrigem >= 0 && verticeOrigem < grafo->numVertices);
    assert(verticeDestino >= 0 && verticeDestino < grafo->numVertices);

    // Verifica se a distância da aresta entre os vértices é diferente de -1
    return (grafo->arestas[verticeOrigem][verticeDestino].distancia != -1);
}

// Adiciona uma aresta ao grafo com a distância especificada
bool adicionarAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino, int distancia) {
    // Verifica se os vértices de origem e destino da aresta e distância da aresta são válidos
    assert(verticeOrigem >= 0 && verticeOrigem < grafo->numVertices);
    assert(verticeDestino >= 0 && verticeDestino < grafo->numVertices);
    assert(distancia >= 0);

    // Verifica se a aresta já existe, nesse caso a aresta não é adicionada
    if (existeAresta(grafo, verticeOrigem, verticeDestino)) {
        return false;
    }

    // Atualiza a distância da aresta no grafo
    grafo->arestas[verticeOrigem][verticeDestino].distancia = distancia;

    return true;
}

// Remove uma aresta do grafo
bool removerAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino) {
    // Verifica se os vértices de origem e destino da aresta são válidos
    assert(verticeOrigem >= 0 && verticeOrigem < grafo->numVertices);
    assert(verticeDestino >= 0 && verticeDestino < grafo->numVertices);

    // Verifica se a aresta não existe, nesse caso a remoção não é possível
    if (!existeAresta(grafo, verticeOrigem, verticeDestino)) {
        return false;
    }

    // Atualiza a distância da aresta no grafo para -1, indicando que a aresta não existe
    grafo->arestas[verticeOrigem][verticeDestino].distancia = -1;

    return true;
}