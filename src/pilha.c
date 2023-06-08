#include <stdlib.h>

#include "../include/grafo.h"
#include "../include/pilha.h"

// Cria uma nova pilha vazia
tPilha *criaPilha(int tamanho_max){
    tPilha *novaPilha = (tPilha*) malloc(sizeof(tPilha));
    novaPilha->items = (int*) malloc(tamanho_max * sizeof(int));

    if (novaPilha == NULL) {
        return NULL;
    }

    novaPilha->topo = -1;

    return novaPilha;
}

//adiciona itens a pilha
void empilhar(tPilha *pilha, int item){
    pilha->topo++;
    pilha->items[pilha->topo] = item;
}

//remove itens da pilha
void desempilhar(tPilha *p){
    int item_remov = p->items[p->topo];
    p->topo--;
}

// destroi pilha
void destruirPilha(tPilha *p) {
    free(p->items);
    p->items = NULL;
    free(p);
    p = NULL;
}