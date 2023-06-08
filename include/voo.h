#pragma once

#include "aeroporto.h"
#include "caminho.h"

#ifndef VOO_H
#define VOO_H

#define VOOS_FILE "dados/voos.csv"
#define MAX_VOOS 100

typedef struct  {
    unsigned int id;
    tAeroporto *aeroportoInicial;
    tAeroporto *aeroportoFinal;
    tCaminho *trajeto;
} tVoo;

tVoo **lerDadosVoos(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numVoos);

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos);

tCaminho *conveterStringParaTrajeto(char *linha, tAeroporto *aeroportos, unsigned int numAeroportos);

void salvarVoo(tVoo *voo, tAeroporto *aeroportos, unsigned int numAeroportos);

void destruirVoo(tVoo *voo);

#endif