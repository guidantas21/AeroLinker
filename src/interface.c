#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../include/aeroporto.h"
#include "../include/interface.h"

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

void cleanCMD() {
    if (CLEAN_CMD) system(CLEAR_SCREEN_COMMAND);
}