#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/aeroporto.h"
#include "../include/debug.h"
#include "../include/utils.h"

tAeroporto *lerDadosAeroportos(unsigned int *numAeroportos) {
    debugAbrirArquivo(AEROPORTOS_FILE, "r");

    FILE *fptr = fopen(AEROPORTOS_FILE, "r");

    if (!verificarArquivo(fptr, AEROPORTOS_FILE)) return NULL;

    // Quantidade de aeroportos listados no arquivo
    *numAeroportos = contarLinhasDeArquivo(fptr);

    tAeroporto *aeroportos = (tAeroporto*) malloc((*numAeroportos) * sizeof(tAeroporto));

    if (aeroportos == NULL) {
        if (DEBUG) printf("Erro ao alocar memória para o vetor de aeroporto\n");
        return NULL;
    }

    // Pega as informações de cada aeroporto e cria um tAeroporto com elas
    int index = 0;
    while (fscanf(fptr,"%[^,],%[^,],%[^,],%[^,],%f,%f\n", 
        aeroportos[index].iata, 
        aeroportos[index].nome, 
        aeroportos[index].local, 
        aeroportos[index].pais,
        &aeroportos[index].latitude,
        &aeroportos[index].longitude) == 6) {
        aeroportos[index].id = index;
        index++;
    }

    debugFecharArquivo(AEROPORTOS_FILE);
    fclose(fptr);

    return aeroportos;
}

void printAeroportos(tAeroporto *aeroportos, unsigned int numAeroportos) {
    printf("\n- Aeroportos cadastrados:\n");
    printf("| IATA  | AEROPORTO\t\t\t | LOCAL\t\t\t | PAÍS\t\t\t\t | LATITUDE\t| LONGITUDE    |\n");
    for (int i = 0; i < numAeroportos; i++) {
        printf("| %3d\t| ", aeroportos[i].id);
        printf("| %3s\t| ", aeroportos[i].iata);
        printf("%-*s\t | ",  25, aeroportos[i].nome);
        printf("%-*s\t | ", 25, aeroportos[i].local);
        printf("%-*s\t | ", 25, aeroportos[i].pais);
        printf("%-*f | ", 12, aeroportos[i].latitude);
        printf("%-*f |\n", 12, aeroportos[i].longitude);
    }
    printf("\n");
}

void destruirAeroportos(tAeroporto **aeroportos, unsigned int *numAeroportos) {
    if (DEBUG) printf("Liberando memória alocada do vetor de aeroportos\n");

    // Libera memória locada para o vetor
    free(*aeroportos);
    *numAeroportos = 0;
    *aeroportos = NULL;
}

tAeroporto *acharAeroportoPorIATA(char iata[], tAeroporto *aeroportos, unsigned numAeroportos) {
    for (int i = 0; i < numAeroportos; i++) {
        if (!strcmp(aeroportos[i].iata, iata) ) {
            debugAcharAeroporto(aeroportos[i], iata);
            return &aeroportos[i];
        }
    }

    if (DEBUG) printf("Aeroporto de IATA '%s' não encontrado\n", iata);

    return NULL;
}

int idAerportoPorIATA(char iata[], tAeroporto *aeroportos, unsigned numAeroportos) {
    for (int i = 0; i < numAeroportos; i++) {
        if (!strcmp(aeroportos[i].iata, iata) ) {
            debugAcharAeroporto(aeroportos[i], iata);
            if (DEBUG) printf("ID: %d\n", i);
            return i;
        }
    }
    if (DEBUG) printf("Aeroporto de IATA '%s' não encontrado\n", iata);
    return -1;
}

char *iataPorId(tAeroporto *aeroporto, int numAeroportos, int id) {
    for (int i = 0; i < numAeroportos; i++) {
        if (idAerportoPorIATA(aeroporto->iata, aeroporto, numAeroportos) == id) {
            return aeroporto->iata;
        }
    }
    return NULL;
}

tAeroporto *aeroportPorId(tAeroporto *aeroportos, int numAeroportos, int id) {
    for (int i = 0; i < numAeroportos; i++) {
        if (aeroportos[i].id == id) {
            return &aeroportos[i];
        }
    }
    return NULL;
}