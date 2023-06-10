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
        voos[index]->horarioSaida = (struct tm*)malloc(sizeof(struct tm));
        
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

        int hora, minuto, dia, mes, ano;

        if (sscanf(horarioLinha, "%d/%d/%d %d:%d", &dia, &mes, &ano, &hora, &minuto) != 5) {
            printf("Erro ao analisar a string de horário.\n");
            return NULL;
        }

        voos[index]->horarioSaida = gerarHorario(hora, minuto, dia, mes, ano);
        voos[index]->id = index;
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

void calcularHorarioChegada(struct tm *horarioChegada, tVoo *voo) {
    time_t timestamp = mktime(voo->horarioSaida); // Converter struct tm para timestamp

    int horasDeVoo = voo->trajeto->menorDistancia / 900; // Tempo que de voo
    int horasConexao = (voo->trajeto->pilha->topo - 2)  * 2; // Tempo em que aviao fica nos aerportos das conexoes 

    // Adicionar horas em segundos ao timestamp
    timestamp += (horasDeVoo + horasConexao) * 3600; // 1 hora = 3600 segundos

    // Converter o novo timestamp de volta para struct tm
    *horarioChegada = *localtime(&timestamp);
}

tVoo *criarVoo(tCaminho *trajeto, tAeroporto *aeroportoInicial, tAeroporto *aeroportoFinal, struct tm *horarioSaida, unsigned int id) {
    tVoo *voo = (tVoo*) malloc(sizeof(tVoo));

    voo->trajeto = trajeto;
    voo->aeroportoInicial = aeroportoInicial;
    voo->aeroportoFinal = aeroportoFinal;
    voo->horarioSaida = horarioSaida;
    voo->id = id;

    return voo;
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

    strftime(horarioString, sizeof(horarioString), "%d/%m/%Y %H:%M", voo->horarioSaida);

    fprintf(fptr, "%s\n", horarioString);

    fclose(fptr);
}

void removerVoo(tVoo **voos, unsigned int *numVoos, unsigned int id) {
    destruirVoo(voos[id]);
    (*numVoos)--;

    for (int j = id; j < *numVoos; j++) {
        voos[j] = voos[j+1];
        voos[j]->id = j;
    }

    voos = realloc(voos, (*numVoos) * sizeof(tVoo*));

    removerLinhaDoAquivo(VOOS_FILE,id);
}

tVoo *acharVooPorId(tVoo **voos, unsigned int numVoos, unsigned int id) {
    if (id < numVoos) 
        return voos[id];

    return NULL;
}

void destruirVoo(tVoo *voo) {
    // Libera a memória do trajeto
    liberarCaminho(voo->trajeto);
    free(voo->horarioSaida);
    // Libera voo
    free(voo);
}