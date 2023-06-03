#include <stdbool.h>

#ifndef GRAFO_H
#define GRAFO_H

typedef struct {
    int distancia; // Distância entre dois vértices
} tAresta;

typedef struct {
    tAresta **arestas; // Matriz adjacente representando as arestas
    int numVertices; // Número de vértices no grafo
} tGrafo;

// Libera toda a memória alocada para o grafo
void liberarGrafo(tGrafo *grafo);
// Cria um novo grafo com o número especificado de vértices
tGrafo *criarGrafo(int numVertices);
// Verifica se uma aresta existe entre dois vértices no grafo
bool existeAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino);
// Adiciona uma aresta ao grafo com a distância especificada
bool adicionarAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino, int distancia);
// Remove uma aresta do grafo
bool removerAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino);

void printArestas(tGrafo *grafo, int numVertices);

#endif