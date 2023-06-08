#pragma once

#include "grafo.h"
#include "pilha.h"

#ifndef CAMINHO_H
#define CAMINHO_H

typedef struct{
    int menorDistancia;
    tPilha *pilha;
} tCaminho;

tCaminho *criaCaminho(unsigned int numVertices);

int verificarNumero(int array[], int tamanho, int numero);

int encontrarVerticeMinimo(int distancia[], int visitado[], int nVertices);

void menorDistancia(tGrafo *grafo, int vInicial, int vFinal, tCaminho *caminho);

void liberarCaminho(tCaminho *caminho);

#endif 