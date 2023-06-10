#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/conexao.h"
#include "../include/aeroporto.h"
#include "../include/grafo.h"

#include "../include/debug.h"
#include "../include/utils.h"

tConexao *lerDadosConexoes(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numConexoes) {
    debugAbrirArquivo(CONEXOES_FILE, "r");

    FILE *fptr = fopen(CONEXOES_FILE, "r");

    if (!verificarArquivo(fptr, CONEXOES_FILE)) return NULL;

    *numConexoes = contarLinhasDeArquivo(fptr);

    tConexao *conexoes = (tConexao*) malloc((*numConexoes) * sizeof(tConexao));

    if (conexoes == NULL) {
        if (DEBUG) printf("Erro ao alocar memória para o vetor de conexões\n");
        return NULL;
    }

    int index = 0;
    char iataInicial[4];
    char iataFinal[4];
    unsigned int distanciaKm;

    while (fscanf(fptr,"%[^,],%[^,],%d\n", iataInicial, iataFinal, &distanciaKm) == 3) {
        conexoes[index].inicial = acharAeroportoPorIATA(iataInicial, aeroportos, numAeroportos);

        if (conexoes[index].inicial == NULL) {
            if (DEBUG) printf("Aeroporto inicial inexistente\n");
            return NULL;
        }

        conexoes[index].final = acharAeroportoPorIATA(iataFinal, aeroportos, numAeroportos);

        if (conexoes[index].final == NULL) {
            if (DEBUG) printf("Aeroporto final inexistente\n");
            return NULL;
        }

        conexoes[index].distanciaKm = distanciaKm;
        
        if (DEBUG) {
            printf("Conexão encontrada: ");
            printf("{ inicial.iata: %s, final.iata: %s, distanciaKm: %d }\n\n", 
                conexoes[index].inicial->iata, 
                conexoes[index].final->iata, 
                conexoes[index].distanciaKm
            );
        }
        index++;
    }

    fclose(fptr);
    debugFecharArquivo(CONEXOES_FILE);

    return conexoes;
}

void destruirConexoes(tConexao **conexoes, unsigned int *numConexoes) {
    if (DEBUG) printf("Liberando memória alocada do vetor de conexões\n");

    free(*conexoes);
    *numConexoes = 0;
    *conexoes = NULL;
}

void passarConexoesParaGrafo(tGrafo *grafo, tConexao *dadosConexoes, int numConexoes, tAeroporto *dadosAeroportos, int numAeroportos) {
    bool check;
    for (int i = 0; i < numConexoes; i++) {
        check = adicionarAresta(grafo, dadosConexoes[i].inicial->id, dadosConexoes[i].final->id, dadosConexoes[i].distanciaKm);

        if (DEBUG) {
            if (check) {
                printf("Conexão adicionada no grafo\n\n");
            } else {
                printf("Conexão inválida!\n\n");
            }
        }
    }
}