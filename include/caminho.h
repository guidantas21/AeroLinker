#include "grafo.h"
#include "pilha.h"

#ifndef CAMINHO_H
#define CAMINHO_H

typedef struct{
    int menorDistancia;
    tPilha pilha;
} tCaminho;

tCaminho *criaCaminho(tGrafo *grafo);

int verificarNumero(int array[], int tamanho, int numero);

int encontrarVerticeMinimo(int distancia[], int visitado[], int nVertices);

void menorDistancia(tGrafo *grafo, int vInicial, int vFinal, tCaminho *caminho);

void calculaDistanciaEntreVertice(tGrafo *grafo, tPilha *pilha, int **vetor);

#endif 