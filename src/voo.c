#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/voo.h"
#include "../include/aeroporto.h"
#include "../include/caminho.h"
#include "../include/debug.h"
#include "../include/utils.h"
#include "../include/pilha.h"

tVoo **lerDadosVoos(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numVoos) {
    int index = 0, distancia;
    char trajetoLinha[256];
    char horarioLinha[20];

    debugAbrirArquivo(VOOS_FILE, "r");

    FILE *fptr = fopen(VOOS_FILE, "r");

    if (!verificarArquivo(fptr, VOOS_FILE)) return NULL;

    // Quantidade de voos listados no arquivo
    *numVoos = contarLinhasDeArquivo(fptr);

    tVoo **voos = (tVoo**) malloc((*numVoos) * sizeof(tVoo*));

    if (voos == NULL) {
        if (DEBUG) printf("Erro ao alocar memória para o vetor de voos\n");
        fclose(fptr);
        return NULL;
    }       
    while (fscanf(fptr,"%d,%[^,],%[^\n]\n", &distancia, trajetoLinha, horarioLinha) == 3) {
        voos[index] = (tVoo*) malloc(sizeof(tVoo));
        
        voos[index]->trajeto = conveterStringParaTrajeto(trajetoLinha, aeroportos, numAeroportos);
        voos[index]->trajeto->menorDistancia = distancia;

        voos[index]->aeroportoInicial = acharAeroportoPorId(
            aeroportos, 
            numAeroportos, 
            voos[index]->trajeto->pilha->items[0]
        );
    
        voos[index]->aeroportoFinal = acharAeroportoPorId(
            aeroportos, 
            numAeroportos, 
            voos[index]->trajeto->pilha->items[voos[index]->trajeto->pilha->topo] // Ultimo item da pilha
        );

        struct tm horario;

        if (sscanf(horarioLinha, "%d:%d", &horario.tm_hour, &horario.tm_min) != 2) {
            printf("Erro ao analisar a string de horário.\n");
            return NULL;
        }

        // Definir outros campos da estrutura que não são fornecidos pela string
        horario.tm_sec = 0;         // Segundos
        horario.tm_mday = 1;        // Dia do mês (apenas para exemplo)
        horario.tm_mon = 0;         // Mês (apenas para exemplo)
        horario.tm_year = 123;      // Ano (apenas para exemplo - 2021 - 1900)
        horario.tm_isdst = -1;      // Horário de verão (informação desconhecida)

        // Converter a estrutura struct tm para um valor time_t
        voos[index]->horarioSaida = mktime(&horario);
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

    int cont = 1;

    for (int i = 0; i < n;  i++) {
        if (linha[i] == '-')
            cont++;
    }

    int numTokens = 0;
    char *delimitador = "-";

    char *token = strtok(linha, delimitador);

    tCaminho *trajeto = (tCaminho*) malloc(sizeof(tCaminho));
    
    if (trajeto == NULL) {
        return NULL;
    }

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
    char horarioString[20];

    printf("\nPartida: %s\nDestino: %s\n",  voo->aeroportoInicial->iata, voo->aeroportoFinal->iata);
    printf("Distância: %d km\n", voo->trajeto->menorDistancia);

    strftime(horarioString, sizeof(horarioString), "%H:%M", localtime(&voo->horarioSaida));

    printf("Horário de saída: %s\n", horarioString);
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

void salvarVoo(tVoo *voo, tAeroporto *aeroportos, unsigned int numAeroportos) {
    FILE *fptr = fopen(VOOS_FILE, "a");
    
    if (!verificarArquivo(fptr, VOOS_FILE)) {
        printf("Nao foi possivel abrir voos\n");
        return;
    }

    fprintf(fptr,"%d,", voo->trajeto->menorDistancia);

    for (int i = 0; i < voo->trajeto->pilha->topo+1; i++) {
        fprintf(fptr, "%s",acharAeroportoPorId(
            aeroportos, 
            numAeroportos, 
            voo->trajeto->pilha->items[i]
        )->iata);

        if (i < voo->trajeto->pilha->topo)
            fprintf(fptr, "-");
    }

    fprintf(fptr,",");

    char horarioString[20];

    strftime(horarioString, sizeof(horarioString), "%H:%M", localtime(&voo->horarioSaida));

    fprintf(fptr, "%s\n", horarioString);

    fclose(fptr);
}

void destruirVoo(tVoo *voo) {
    // Libera a memória do trajeto
    liberarCaminho(voo->trajeto);

    // Libera voo
    free(voo);
}