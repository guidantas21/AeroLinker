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

void destruirAeroportos(tAeroporto **aeroportos, unsigned int *numAeroportos) {
    if (DEBUG) printf("Liberando memória alocada do vetor de aeroportos\n");

    // Libera memória locada para o vetor
    free(*aeroportos);
    *numAeroportos = 0;
    *aeroportos = NULL;
}

bool compararIata(char iata1[4], char iata2[4]) {
    int n = strlen(iata1);

    for (int i = 0; i < n; i++) {
        if (iata1[i] != iata2[i]) {
            return false;
        }
    }

    return true;
}

tAeroporto *acharAeroportoPorIATA(char iata[], tAeroporto *aeroportos, unsigned int numAeroportos) {
    for (int i = 0; i < numAeroportos; i++) {
        if (compararIata(aeroportos[i].iata, iata)) {
            debugAcharAeroporto(aeroportos[i], iata); 
            return &aeroportos[i];
        }
    }
    
    if (DEBUG) printf("\nAeroporto de IATA '%s' não encontrado\n", iata);

    return NULL;
}

tAeroporto *acharAeroportoPorId(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int id) {
    if (id < numAeroportos) 
        return &aeroportos[id];

    return NULL;
}
