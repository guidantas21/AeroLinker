#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "../include/caminho.h"
#include "../include/grafo.h"
#include "../include/pilha.h"

//cria caminho
tCaminho *criaCaminho(tGrafo *grafo){
    tCaminho *novoCaminho = (tCaminho*)malloc(sizeof(tCaminho));
    if (novoCaminho == NULL) {
        return NULL;
    }
    
    novoCaminho->menorDistancia = 0;
    novoCaminho->pilha = criaPilha(grafo);
    
    return novoCaminho;
}

//liberando memoria alocada para caminho
void liberarCaminho(tCaminho *caminho) {
  free(caminho->pilha->items);
  free(caminho);
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
    //inicio da declaração de variavel//
    int estimativas[grafo->numVertices];
    int precedentes[grafo->numVertices];
    int visitados[grafo->numVertices];
    int vAnalise;

    tPilha *tempPilha = criaPilha(grafo);
  
    //calculando o menor caminho do verticie inicial para os demais//
    for (int v = 0; v < grafo->numVertices; v++) {
        estimativas[v] = INT_MAX;
        visitados[v] = 0;
    }
  
    estimativas[vInicial]=0;
    precedentes[vInicial]=0;

    for (int c=0; c < grafo->numVertices - 1; c++) {
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

    while(vAnalise!=vInicial){
        empilhar(tempPilha, precedentes[vAnalise]);
        vAnalise=precedentes[vAnalise];
    }

    empilhar(tempPilha, vInicial);

    for(int i=tempPilha->topo-1; i>=0; i--){
        empilhar(caminho->pilha, tempPilha->items[i]);
    }
}

void calculaDistanciaEntreVertice(tGrafo *grafo, tPilha *pilha, int **vetor){
    int i, j, v, f;

    int tamanho = pilha->topo - 1;
    *vetor = (int*)malloc(tamanho * sizeof(int));
  
    for (i =0, j =1 ; j<=pilha->topo; i++, j++){
        v = pilha->items[i];
        f = pilha->items[j];
        (*vetor)[i] = grafo->arestas[v][f].distancia;
    }
}