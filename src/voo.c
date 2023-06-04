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

void printVooInfo(tVoo *voo) {
    printf("Partida: %s\nDestino: %s\n",  voo->aeroportoInicial->iata, voo->aeroportoFinal->iata);
    printf("Distância: %d km\n", voo->trajeto->menorDistancia);

    printf("Trajeto: ");
    for (int i = 0; i < voo->trajeto->pilha.topo + 1; i++) {
        printf("%d", voo->trajeto->pilha.items[i]);

        if (i != voo->trajeto->pilha.topo)
            printf(" >> ");
    }
    printf("\n");
}

void destruirVoo(tVoo *voo) {
    // Libera a memória do trajeto
    liberarCaminho(voo->trajeto);

    // Libera a memória do struct tVoo
    free(voo);
}