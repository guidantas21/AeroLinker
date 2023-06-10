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
    printf("Menu\n");
    printf("1. Exibir dados dos aeroportos\n");
    printf("2. Exibir dados das conexões\n");
    printf("3. Exibir matriz de conexões de aeroportos\n");
    printf("4. Exibir mapa da rede aérea\n");
    printf("5. Exibir mapa de todos os voos\n");
    printf("6. Exibir mapa de voo específico\n");
    printf("7. Cadastrar voos\n");
    printf("8. Remover voos\n");
    printf("9. Ver voos cadastrados\n");
    printf("0. Sair do programa\n\n");
}

int inputOpcao() {
    int opcao;
    scanf("%d", &opcao);

    return opcao;
}

void perguntaAeroporto(char iataInicial[], char iataFinal[]) {
    printf ("\nAeroporto Inicial (IATA): "); 
    scanf("%s", iataInicial);
    printf ("Aeroporto Final (IATA): "); 
    scanf("%s", iataFinal);
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

void printConexoes(tConexao *conexoes, unsigned int numConexoes) {
    printf("\n- Conexões cadastradas:\n");
    printf("| INÍCIO |  FIM\t | DISTÂNCIA (KM)  |\n");
    for (int i = 0; i < numConexoes; i++) {
        printf("|  %3s\t ", conexoes[i].inicial->iata);
        printf("|  %3s\t | \t", conexoes[i].final->iata);
        printf("%-*d |\n", 10, conexoes[i].distanciaKm);
    }
    printf("\n");
}

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos) {
    tAeroporto *aeroporto;
    char horarioString[30];
    struct tm horarioChegada;

    printf("\nPartida: %s\nDestino: %s\n",  voo->aeroportoInicial->iata, voo->aeroportoFinal->iata);
    printf("Distância: %d km\n", voo->trajeto->menorDistancia);

    strftime(horarioString, sizeof(horarioString), "%d/%m/%Y %H:%M", voo->horarioSaida);

    printf("Horário de saída: %s\n", horarioString);

    calcularHorarioChegada(&horarioChegada, voo);

    strftime(horarioString, sizeof(horarioString), "%d/%m/%Y %H:%M", &horarioChegada);

    printf("Horário de chegada: %s\n", horarioString);
    
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

void printArestas(tGrafo *grafo, int numVertices) {
	printf("\n- Distâncias\n");
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            printf("%7d ", grafo->arestas[i][j].distancia);
        }
        printf("\n");
    }
}

void cleanCMD() {
    if (CLEAN_CMD) system(CLEAR_SCREEN_COMMAND);
}