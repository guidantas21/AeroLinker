#pragma once

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

tAeroporto *acharAeroportoPorIATA(char iata[], tAeroporto *aeroportos, unsigned numAeroportos);

int idAerportoPorIATA(char iata[], tAeroporto *aeroportos, unsigned numAeroportos);

char *iataPorId(tAeroporto *aeroporto, int numAeroportos, int id);

tAeroporto *aeroportPorId(tAeroporto *aeroportos, int numAeroportos, int id);

#endif /* AEROPORTOS_H */