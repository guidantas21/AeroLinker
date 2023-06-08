#pragma once

#include <stdbool.h>

#ifndef AEROPORTOS_H
#define AEROPORTOS_H

#define AEROPORTOS_FILE "dados/aeroportos.csv"

typedef struct {
    unsigned int id;
    char iata[4];
    char nome[100];
    char local[100];
    char pais[100];
    float latitude;
    float longitude;
} tAeroporto;

tAeroporto *lerDadosAeroportos(unsigned int *numAeroportos);

void printAeroportos(tAeroporto *aeroportos, unsigned int numAeroportos);

void destruirAeroportos(tAeroporto **aeroportos, unsigned int *numAeroportos);

bool compararIata(char iata1[4], char iata2[4]);

tAeroporto *acharAeroportoPorIATA(char iata[], tAeroporto *aeroportos, unsigned int numAeroportos);

tAeroporto *acharAeroportoPorId(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int id);

#endif