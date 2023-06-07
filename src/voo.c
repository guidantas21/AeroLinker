#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/voo.h"
#include "../include/aeroporto.h"
#include "../include/caminho.h"
#include "../include/debug.h"
#include "../include/utils.h"
#include "../include/pilha.h"

tVoo *lerDadosVoos(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numVoos) {
    debugAbrirArquivo(VOOS_FILE, "r");

    FILE *fptr = fopen(VOOS_FILE, "r");

    if (!verificarArquivo(fptr, VOOS_FILE)) return NULL;

    // Quantidade de voos listados no arquivo
    *numVoos = contarLinhasDeArquivo(fptr);

    tVoo *voos = (tVoo*) malloc((*numVoos) * sizeof(tVoo));

    if (voos == NULL) {
        if (DEBUG) printf("Erro ao alocar memória para o vetor de voos\n");
        return NULL;
    }
    
    char *linha;
    int menorDistancia;
    int index = 0;
    while (fscanf(fptr,"%d,%m[^\n]\n", &menorDistancia, &linha) == 2) {
        voos[index].trajeto = conveterStringParaTrajeto(linha, aeroportos, numAeroportos);
        voos[index].trajeto->menorDistancia = menorDistancia;
        voos[index].aeroportoInicial = acharAeroportoPorId(
            aeroportos, 
            numAeroportos, 
            voos[index].trajeto->pilha->items[0]
        );
        voos[index].aeroportoFinal = acharAeroportoPorId(
            aeroportos, 
            numAeroportos, 
            voos[index].trajeto->pilha->items[voos[index].trajeto->pilha->topo] // Ultimo item da pilha
        );
        free(linha);
        index++;
    }

    fclose(fptr);

    return voos;
}

tCaminho *conveterStringParaTrajeto(char *linha, tAeroporto *aeroportos, unsigned int numAeroportos) {
    int n = strlen(linha);

    if (n == 0) {
        return NULL;
    }

    int i = 0, cont = 1;

    for (int i = 0; i < n;  i+=3)
        if (linha[i] == '-')
            cont++;

    printf("%d\n", cont);

    char **tokens = (char**) malloc(cont * sizeof(char*));
    int numTokens = 0;
    char *delimitador = "-";

    char *token = strtok(linha, delimitador);

    tCaminho *trajeto = (tCaminho*) malloc(sizeof(tCaminho));
    
    if (trajeto == NULL) {
        return NULL;
    }
    
    trajeto->menorDistancia = 0;
    trajeto->pilha = criaPilha(cont);

    while (token != NULL && numTokens < cont) {
        // Empilha id do aeroporto
        empilhar(trajeto->pilha, acharAeroportoPorIATA(token, aeroportos, numAeroportos)->id);
        // Separa a próxima string
        token = strtok(NULL, delimitador);
        numTokens++;
    }

    return trajeto;
}

tVoo *criarVoo(tAeroporto *aeroportoInicial, tAeroporto *aeroportoFinal, tCaminho *trajeto) {
    tVoo *novoVoo = (tVoo*) malloc(sizeof(tVoo));

    novoVoo->aeroportoInicial = aeroportoInicial;
    novoVoo->aeroportoFinal = aeroportoFinal;

    novoVoo->trajeto = trajeto;

    return novoVoo;
}

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos) {
    tAeroporto *aeroporto;
    printf("\nPartida: %s\nDestino: %s\n",  voo->aeroportoInicial->iata, voo->aeroportoFinal->iata);
    printf("Distância: %d km\n", voo->trajeto->menorDistancia);

    printf("Trajeto: ");
    for (int i = 0; i < voo->trajeto->pilha->topo + 1; i++) {
        aeroporto = acharAeroportoPorId(aeroportos, numAeroportos, voo->trajeto->pilha->items[i]);

        if (aeroporto != NULL) {
            printf("%s", aeroporto->iata);
            if (aeroporto->id != voo->aeroportoFinal->id) {
                printf(" >> ");
            }
        } else {
            printf("Aerporto não encontrado.\n");
        }
    }
    printf("\n\n");
}

void destruirVoo(tVoo *voo) {
    // Libera a memória do trajeto
    liberarCaminho(voo->trajeto);

    // Libera a memória do struct tVoo
    free(voo);
}