#include "aeroporto.h"
#include "caminho.h"

#ifndef VOO_H
#define VOO_H

typedef struct  {
    tAeroporto aeroportoInicial;
    tAeroporto aeroportoFinal;
    tCaminho trajeto;
} tVoo;

#endif