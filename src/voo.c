#include <stdio.h>
#include <stdlib.h>

#include "../include/voo.h"
#include "../include/aeroporto.h"
#include "../include/caminho.h"

tVoo *criarVoo(tAeroporto *aeroportoInicial, tAeroporto *aeroportoFinal, tCaminho *trajeto) {
    tVoo *novoVoo = (tVoo*) malloc(sizeof(tVoo));

    novoVoo->aeroportoInicial = aeroportoInicial;
    novoVoo->aeroportoFinal = aeroportoFinal;

    novoVoo->trajeto = trajeto;
}

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos) {
    tAeroporto *aeroporto;
    printf("\nPartida: %s\nDestino: %s\n",  voo->aeroportoInicial->iata, voo->aeroportoFinal->iata);
    printf("Distância: %d km\n", voo->trajeto->menorDistancia);

    printf("Trajeto: ");
    for (int i = 0; i < voo->trajeto->pilha.topo + 1; i++) {
        aeroporto = aeroportPorId(aeroportos, numAeroportos, voo->trajeto->pilha.items[i]);

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