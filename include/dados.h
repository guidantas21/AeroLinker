#pragma once

#include <time.h>

#include "aeroporto.h"
#include "caminho.h"
#include "conexao.h"
#include "voo.h"

#ifndef DADOS_H
#define DADOS_H

typedef struct {
    tAeroporto *aeroportos; // Lista de aeroportos
    unsigned int numAeroportos;
    tConexao *conexoes; // Lista de conexoes
    unsigned int numConexoes;
    tVoo **voos; // Lista de voos
    unsigned int numVoos;
} tDados;

#endif