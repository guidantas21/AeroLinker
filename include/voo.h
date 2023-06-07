#pragma once

#include "aeroporto.h"
#include "caminho.h"

#ifndef VOO_H
#define VOO_H

#define VOOS_FILE "dados/voos.csv"

typedef struct  {
    unsigned int id;
    tAeroporto *aeroportoInicial;
    tAeroporto *aeroportoFinal;
    tCaminho *trajeto;
} tVoo;

tVoo *lerDadosVoos(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numVoos);

tVoo *criarVoo(tAeroporto *aeroportoInicial, tAeroporto *aeroportoFinal, tCaminho *trajeto);

// void adicionarVoo(tVoo ***voos, unsigned int *numVoos, tAeroporto *aeroportoInicial, tAeroporto *aeroportoFinal, tCaminho *trajeto);

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos);

tCaminho *conveterStringParaTrajeto(char *linha, tAeroporto *aeroportos, unsigned int numAeroportos);

void destruirVoo(tVoo *voo);

#endif