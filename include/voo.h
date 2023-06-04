#pragma once

#include "aeroporto.h"
#include "caminho.h"

#ifndef VOO_H
#define VOO_H

#define VOOS_FILE "dados/aeroportos.csv"

typedef struct  {
    unsigned int id;
    tAeroporto *aeroportoInicial;
    tAeroporto *aeroportoFinal;
    tCaminho *trajeto;
} tVoo;

tVoo *criarVoo(tAeroporto *aeroportoInicial, tAeroporto *aeroportoFinal, tCaminho *trajeto);

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos);

void destruirVoo(tVoo *voo);

#endif