#pragma once

#ifndef PILHA_H
#define PILHA_H

typedef struct{
    int topo;
    int *items;
} tPilha;

// Cria uma nova pilha vazia
tPilha *criaPilha(tGrafo *grafo);

//adiciona itens a pilha
void empilhar(tPilha *pilha, int item);

//remove itens da pilha
void desempilhar(tPilha *p);

#endif