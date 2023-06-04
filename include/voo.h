#pragma once

#include "aeroporto.h"
#include "caminho.h"

#ifndef VOO_H
#define VOO_H

typedef struct  {
    tAeroporto *aeroportoInicial;
    tAeroporto *aeroportoFinal;
    tCaminho *trajeto;
} tVoo;

tVoo *criarVoo(tAeroporto *aeroportoInicial, tAeroporto *aeroportoFinal, tCaminho *trajeto);

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos);

void destruirVoo(tVoo *voo);

#endif