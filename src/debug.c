#include <stdio.h>
#include <stdbool.h>

#include "../include/debug.h"
#include "../include/aeroporto.h"

void debugAbrirArquivo(char endereco[], char modo[]) {
    if (DEBUG) printf("\nAbrindo '%s' no modo '%s'\n", endereco, modo);
}

void debugFecharArquivo(char endereco[]) {
    if (DEBUG) printf("\nFechando o arquivo '%s'\n", endereco);
}

void debugIniciarArquivo(char endereco[]) {
    if (DEBUG) printf("Arquivo '%s' iniciado com sucesso\n", endereco);
}

void debugAcharAeroporto(tAeroporto aeroporto, char iata[]) {
    if (DEBUG) {
        printf("Aeroporto de IATA '%s' encontrado: ", iata);
        printf("{ iata: %s, nome: %s, local: %s, pais: %s, latitude: %f, longitude: %f }\n", 
            aeroporto.iata, 
            aeroporto.nome, 
            aeroporto.local, 
            aeroporto.pais,
            aeroporto.latitude,
            aeroporto.longitude
        );
    }
}