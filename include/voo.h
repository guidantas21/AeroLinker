#pragma once

#include <time.h>

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
    struct tm *horarioSaida;
} tVoo;

tVoo **lerDadosVoos(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numVoos);

tCaminho *conveterStringParaTrajeto(char *linha, tAeroporto *aeroportos, unsigned int numAeroportos);

tVoo *criarVoo(tCaminho *trajeto, tAeroporto *aeroportoInicial, tAeroporto *aeroportoFinal, struct tm *horarioSaida, unsigned int id);

void salvarVoo(tVoo *voo, tAeroporto *aeroportos, unsigned int numAeroportos);

void calcularHorarioChegada(struct tm *horarioChegada, tVoo *voo);

void removerVoo(tVoo **voos, unsigned int *numVoos, unsigned int id);

tVoo *acharVooPorId(tVoo **voos, unsigned int numVoos, unsigned int id);

void destruirVoo(tVoo *voo);

#endif