#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../include/aeroporto.h"
#include "../include/conexao.h"
#include "../include/caminho.h"
#include "../include/voo.h"
#include "../include/interface.h"
#include "../include/grafo.h"
#include "../include/dados.h"

void mostrarMapa(char *protocolo) {
    if (getenv("WSL_DISTRO_NAME") == NULL) {
        char comando[] = COMANDO_MAPA_PYTHON;
        system(strcat(comando, protocolo));
    } else {
        printf("Está sendo executado no Windows Subsystem for Linux (WSL).\n");
    }
} 

void mostrarMapaRedeAerea() {
    mostrarMapa("rede");
} 

void mostrarMapaVoos() {
    mostrarMapa("voos");
} 

void mostrarMapaVoo(char *pk) {
    mostrarMapa(pk);
} 

void print_logo(char endereco[]){
    FILE *fptr = fopen(endereco,"r");
}

void splashScreen(int duracao) {
    printf("                                  _      _       _             \n");
    printf("              /\\                 | |    (_)     | |            \n");
    printf("             /  \\   ___ _ __ ___ | |     _ _ __ | | _____ _ __ \n");
    printf("            / /\\ \\ / _ \\ '__/ _ \\| |    | | '_ \\| |/ / _ \\ '__|\n");
    printf("           / ____ \\  __/ | | (_) | |____| | | | |   <  __/ |   \n");
    printf("          /_/    \\_\\___|_|  \\___/|______|_|_| |_|_|\\_\\___|_|   \n");
    printf("                                                             \n");
    printf("                                                             \n");
    printf("                                    |                        \n");
    printf("                                    |                        \n");
    printf("                                    |                        \n");
    printf("                                  .-'-.\n");
    printf("                                 ' ___ '\n");
    printf("                       ---------'  .-.  '---------\n");
    printf("       _________________________'  '-'  '_________________________\n");
    printf("        ''''''-|---|--/    \\==][^',_m_,'^][==/    \\--|---|-''''''\n");
    printf("                      \\    /  ||/   H   \\||  \\    /\n");
    printf("                       '--'   OO   O|O   OO   '--'\n");

    time_t startTime = time(NULL);
    while (time(NULL) - startTime < duracao);
}

void printMenu() {
    printf("\n>>> MENU <<<\n");
    printf("1. Exibir dados\n");
    printf("2. Exibir mapas\n");
    printf("3. Cadastrar voo\n");
    printf("4. Pesquisar voo\n");
    printf("0. Sair do programa\n");
}

char inputOpcao() {
    char opcao;
    printf("\n>> Selecione uma opção: ");
    scanf(" %c", &opcao);
    printf("\n");

    return opcao;
}

void perguntaAeroporto(char iataInicial[], char iataFinal[]) {
    printf ("\nAeroporto Inicial (IATA): "); 
    scanf("%s", iataInicial);
    printf ("Aeroporto Final (IATA): "); 
    scanf("%s", iataFinal);
}

void printAeroportos(tAeroporto *aeroportos, unsigned int numAeroportos) {
    printf("\n>> Aeroportos cadastrados:\n");
    printf("| IATA  | AEROPORTO\t\t\t | LOCAL\t\t\t | PAÍS\t\t\t | LATITUDE\t| LONGITUDE    |\n");
    for (int i = 0; i < numAeroportos; i++) {
        printf("| %3s\t| ", aeroportos[i].iata);
        printf("%-*s\t | ",  25, aeroportos[i].nome);
        printf("%-*s\t | ", 25, aeroportos[i].local);
        printf("%-*s\t | ", 25, aeroportos[i].pais);
        printf("%-*f | ", 12, aeroportos[i].latitude);
        printf("%-*f |\n", 12, aeroportos[i].longitude);
    }
    printf("\n\n");
}

void printConexoes(tConexao *conexoes, unsigned int numConexoes) {
    printf("\n>> Conexões cadastradas:\n");
    printf("| INÍCIO |  FIM\t | DISTÂNCIA (KM)  |\n");
    for (int i = 0; i < numConexoes; i++) {
        printf("|  %3s\t ", conexoes[i].inicial->iata);
        printf("|  %3s\t | \t", conexoes[i].final->iata);
        printf("%-*d |\n", 10, conexoes[i].distanciaKm);
    }
    printf("\n\n");
}

void printVoos(tVoo **voos, unsigned int numVoos, tAeroporto *aeroportos, unsigned int numAeroportos) {
    char horarioString[30];

    printf("\n>> Voos cadastrados:\n");
    printf("|   ID   | \t\tINICIO \t\t | \t\tDESTINO \t |  DISTANCIA  |      DATA E HORA       |  TRAJETO\n");
    for (int i = 0; i < numVoos; i++) {
        printf("|  %3d   | ", voos[i]->id);
        printf("%-*s\t | ", 25, voos[i]->aeroportoInicial->local);
        printf("%-*s\t | ", 25, voos[i]->aeroportoFinal->local);
        printf(" %5d km   | ", voos[i]->trajeto->menorDistancia);
        strftime(horarioString, sizeof(horarioString), " %d/%m/%Y - %H:%M   |", voos[i]->horarioSaida);
        printf(" %s", horarioString);
        for (int j = 0; j < voos[i]->trajeto->pilha->topo+1; j++) {
            printf(" %s ", acharAeroportoPorId(
                aeroportos, 
                numAeroportos, 
                voos[i]->trajeto->pilha->items[j])->iata
            );

            if (j < voos[i]->trajeto->pilha->topo) {
                printf(" >> ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos) {
    tAeroporto *aeroporto;

    char horarioString[30];
    struct tm horarioChegada;

    printf("\n\n<< Partida >>\n");
    printf("Aeroporto: (id: %d) %s - %s\n", voo->aeroportoInicial->id, voo->aeroportoInicial->iata, voo->aeroportoInicial->nome);
    printf("Local: %s, %s\n", voo->aeroportoInicial->local, voo->aeroportoInicial->pais);
    strftime(horarioString, sizeof(horarioString), "%d/%m/%Y as %H:%M", voo->horarioSaida);
    printf("Data e horário da decolagem: %s\n\n", horarioString);

    printf(">> Destino <<\n");
    printf("Aeroporto: (id: %d) %s - %s\n", voo->aeroportoFinal->id, voo->aeroportoFinal->iata, voo->aeroportoFinal->nome);
    printf("Local: %s, %s\n", voo->aeroportoFinal->local, voo->aeroportoFinal->pais);
    calcularHorarioChegada(&horarioChegada, voo);
    strftime(horarioString, sizeof(horarioString), "%d/%m/%Y as %H:%M", &horarioChegada);
    printf("Data e horário de chegada: %s\n\n", horarioString);

    printf("== Trajeto ==\n");
    printf("Distância: %d km\n", voo->trajeto->menorDistancia);
    printf("Trajeto: ");
    for (int i = 0; i < voo->trajeto->pilha->topo + 1; i++) {
        aeroporto = acharAeroportoPorId(aeroportos, numAeroportos, voo->trajeto->pilha->items[i]);

        if (aeroporto != NULL) {
            printf("%s (%s)", aeroporto->local, aeroporto->iata);
            if (aeroporto->id != voo->aeroportoFinal->id) {
                printf(" >>> ");
            }
        } else {
            printf("Aerporto não encontrado.\n");
        }
    }
    printf("\n\n");
}

void printArestas(tGrafo *grafo, int numVertices) {
	printf("\n>> Matrix de conexões ([inicio][destino] = distância)\n\n");
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            printf("%7d ", grafo->arestas[i][j].distancia);
        }
        printf("\n\n");
    }
    printf("\n");
}

void inputData(int *dia, int *mes, int *ano) {
    while (true) {
        printf("Data (dia/mes/ano): ");
        scanf(" %d/%d/%d", dia, mes, ano);

        if (1 <= *dia && *dia <= 31 && 1 <= *mes && *mes <= 12)
            break;
        else {
            printf("Data inválida! Tente novamente.\n");
        }
    }
}

void inputHorario(int *hora, int* minuto) {
    while (true) {
        printf("Horário (hora:minuto): ");
        scanf(" %d:%d", hora, minuto);

        if (0 <= *hora && *hora < 24 && 0 <= *minuto && *minuto < 60)
            break;
        else {
            printf("Horário inválido! Tente novamente.\n");
        }
    }
}

char* inputIdStr() {
    char* idStr = malloc(4 * sizeof(char));
    unsigned int id = inputId();

    snprintf(idStr, sizeof(idStr ), "%d", id);

    return idStr;
}

unsigned int inputId() {
    unsigned int id;

    printf(">> Id: ");
    scanf(" %d", &id);

    return id;
}

void menuMapas() {
    bool rodando = true;

    while (rodando) {
        printf(
            ">> Opções:\n"
            "1. Mapa da rede aérea\n"
            "2. Mapa de voos\n"
            "3. Mapa de voo específico\n"
            "0. Voltar\n"
        );

        switch (inputOpcao()) {
            case '1':
                mostrarMapaRedeAerea();
                break;
            case '2':
                mostrarMapaVoos();
                break;   
            case '3':
            {
                char idStr[4];

                printf("Id: ");
                scanf("%s", idStr);

                mostrarMapaVoo(idStr);

                break;
            }         
            case '0':
                rodando = false;
                break;
            
            default:
                printf("\nSelecione uma opção válida\n");
                break;
        }
        cleanCMD();
    }
}

void menuVoo(tVoo **voos, unsigned int *numVoos, unsigned int id) {
    bool rodando = true;

    while (rodando) {
        printf(
            ">> Opções:\n"
            "1. Ver no mapa\n"
            "2. Remover\n"
            "0. Voltar\n"
        );

        switch (inputOpcao()) {
            case '1':
            {
                char idStr[4];

                snprintf(idStr, sizeof(idStr), "%d", id);
                mostrarMapaVoo(idStr);
                break;
            }
            case '2':
                removerVoo(voos, numVoos, id);
                rodando = false;
                break;
            case '0':
                rodando = false;
                break;
        
            default:
                printf("\nSelecione uma opção válida\n");
                break;
        }
    }
}

void exibirDados(tDados dados, tGrafo *redeAeroportos) {
    bool rodando = true;

    while (rodando) {
        printf(
            ">> Opções:\n"
            "1. Voos\n"
            "2. Aeroportos\n"
            "3. Conexões\n"
            "4. Print matriz de conexões\n"
            "0. Voltar\n"
        );

        switch (inputOpcao()) {
            case '1':
                printVoos(dados.voos, dados.numVoos, dados.aeroportos, dados.numAeroportos);
                break;
            case '2':
                printAeroportos(dados.aeroportos, dados.numAeroportos);
                break;
            case '3':
                printConexoes(dados.conexoes, dados.numConexoes);
                break;
            case '4':
                printArestas(redeAeroportos, redeAeroportos->numVertices);
                break;
            case '0':
                rodando = false;
                break;
            default:
                printf("\nSelecione uma opção válida\n");
                break;
        }
    }
}

void pesquisarVoo(tVoo **voos, unsigned int *numVoos, tAeroporto *aeroportos, unsigned int numAeroportos) {
    bool rodando = true;

    while (rodando) {
        printf(
            ">> Pesquisar voo por:\n"
            "1. Id\n"
            "0. Voltar\n"
        );

        switch (inputOpcao()) {
            case '1':
            {
                unsigned int id = inputId();

                tVoo *voo = acharVooPorId(voos, *numVoos, id);

                printVooInfo(voo, aeroportos, numAeroportos);

                menuVoo(voos, numVoos, id);
                break;
            }
            case '0':
                rodando = false;
                break;
        
            default:
                printf("\nSelecione uma opção válida\n");
                break;
        }
    }
}

void cleanCMD() {
    if (CLEAN_CMD) system(CLEAR_SCREEN_COMMAND);
}