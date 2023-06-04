#pragma once

#ifndef CONEXAO_H
#define CONEXAO_H

#include "aeroporto.h"
#include "grafo.h"

#define CONEXOES_FILE "dados/conexoes.csv"

typedef struct {
    tAeroporto *inicial;
    tAeroporto *final;
    unsigned int distanciaKm;
} tConexao;

tConexao *lerDadosConexoes(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numConexoes);

void printConexoes(tConexao *conexoes, unsigned int numConexoes);

void destruirConexoes(tConexao **conexoes, unsigned int *numConexoes);

void passarConexoesParaGrafo(tGrafo *grafo, tConexao *dadosConexoes, int numConexoes, tAeroporto *dadosAeroportos, int numAeroportos);

#endif