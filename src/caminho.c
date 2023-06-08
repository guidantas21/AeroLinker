#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../include/caminho.h"
#include "../include/grafo.h"
#include "../include/pilha.h"

//cria caminho
tCaminho *criaCaminho(unsigned int numVertices) {
    tCaminho *novoCaminho = (tCaminho*) malloc(sizeof(tCaminho));
    
    if (novoCaminho == NULL) {
        return NULL;
    }
    
    novoCaminho->menorDistancia = 0;
    novoCaminho->pilha = criaPilha(numVertices);
    
    return novoCaminho;
}

//liberando memoria alocada para caminho
void liberarCaminho(tCaminho *caminho) {
  destruirPilha(caminho->pilha);
  free(caminho);
  caminho = NULL;
}

int verificarNumero(int array[], int tamanho, int numero) {
    for (int i = 0; i < tamanho; i++) {
        if (array[i] == numero) {
            return 1; 
        }
    }
    return 0; 
}

int encontrarVerticeMinimo(int distancia[], int visitado[], int nVertices) {
    int minimo = INT_MAX, indiceMinimo;

    for (int v = 0; v < nVertices; v++) {
        if (!visitado[v] && distancia[v] <= minimo) {
            minimo = distancia[v];
            indiceMinimo = v;
        }
    }

    return indiceMinimo;
}

void menorDistancia(tGrafo *grafo, int vInicial, int vFinal, tCaminho *caminho){
    //inicio da declaração de variavel
    int *estimativas = (int*) malloc(grafo->numVertices * sizeof(int));
    int *precedentes = (int*) malloc(grafo->numVertices * sizeof(int));
    int *visitados = (int*) malloc(grafo->numVertices * sizeof(int));

    int vAnalise;

    tPilha *tempPilha = criaPilha(grafo->numVertices);
  
    //calculando o menor caminho do verticie inicial para os demais
    for (int v = 0; v < grafo->numVertices; v++) {
        estimativas[v] = INT_MAX;
        visitados[v] = 0;
    }
  
    estimativas[vInicial]=0;
    precedentes[vInicial]=0;

    for (int c = 0; c < grafo->numVertices - 1; c++) {
        int vMin = encontrarVerticeMinimo(estimativas, visitados, grafo->numVertices);
        visitados[vMin] = 1;

        for (int j = 0; j < grafo->numVertices; j++) {
          
            if (!visitados[j] && grafo->arestas[vMin][j].distancia !=-1 && estimativas[vMin] != INT_MAX
                && estimativas[vMin] + grafo->arestas[vMin][j].distancia < estimativas[j]) {
                  estimativas[j] = estimativas[vMin] + grafo->arestas[vMin][j].distancia;
                  precedentes[j] = vMin;
            }
        }
    }

    caminho->menorDistancia=estimativas[vFinal];

    //Adicionando em uma pilha o caminho
    empilhar(tempPilha, vFinal);
    vAnalise = vFinal;

    while(vAnalise != vInicial){
        empilhar(tempPilha, precedentes[vAnalise]);
        vAnalise = precedentes[vAnalise];
    }

    empilhar(tempPilha, vInicial);

    for(int i = tempPilha->topo-1; i >= 0; i--){
        empilhar(caminho->pilha, tempPilha->items[i]);
    }

    // Liberar arrays auxiliares
    free(estimativas);
    free(precedentes);
    free(visitados);

    destruirPilha(tempPilha);
}