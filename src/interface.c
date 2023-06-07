#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../include/aeroporto.h"
#include "../include/interface.h"

void mostrarMapaRedeAerea() {
    if (getenv("WSL_DISTRO_NAME") == NULL) {
        system(COMANDO_MAPA_PYTHON);
    } else {
        printf("Está sendo executado no Windows Subsystem for Linux (WSL).\n");
    }
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
    printf("1. dados dos aeroportos\n");
    printf("2. dados das conexões\n");
    printf("3. printar arestas\n");
    printf("4. mapa da rede aérea\n");
    printf("5. cadastrar voos\n");
    printf("6. ver voos\n");
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

void cleanCMD() {
    if (CLEAN_CMD) system(CLEAR_SCREEN_COMMAND);
}