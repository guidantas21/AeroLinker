#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "../include/debug.h"
#include "../include/caminho.h"

bool verificarArquivo(FILE *fptr, char endereco[]) {
    if (fptr == NULL) {
        if (DEBUG) printf("Erro ao abrir o arquivo '%s'\n", endereco);
        return false;
    }
    return true;
}

bool iniciarArquivo(char endereco[]) {
    debugAbrirArquivo(endereco, "a");

    // Se o arquivo não existir, será criado
    FILE *fptr = fopen(endereco, "a");

    if (!verificarArquivo(fptr, endereco)) return false;

    debugIniciarArquivo(endereco);

    debugFecharArquivo(endereco);
    fclose(fptr);

    return true;
}

unsigned int contarLinhasDeArquivo(FILE *fptr) {
    unsigned int c = 0;
    char linha[200];

    // Conta a quantidade de linhas do arquivo
    while (fgets(linha, sizeof(linha), fptr) != NULL)
        c++;

    // Volta ao início do arquivo
    fseek(fptr, 0, SEEK_SET);

    if (DEBUG) printf("O arquivo possui %d linhas\n", c);

    return c;
}

char *stringMaiuscula(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}

void removerLinhaDoAquivo(char endereco[], unsigned int linhaDelete)  {
    char enderecoTemp[25] = "__temp__.csv";
    char linha[1024];

    FILE *arquivo = fopen(endereco, "r");

    if (arquivo  == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    FILE *arquivoTemp = fopen(enderecoTemp, "w");

    if (arquivo  == NULL) {
        printf("Erro ao abrir arquivo temporário\n");
        return;
    }

    unsigned int linhaContador = 0;

    while (true) {
        fgets(linha, 1024, arquivo);
        
        if (feof(arquivo))
            break;
        else if (linhaContador != linhaDelete) {
            
            fputs(linha,arquivoTemp);
        }

        linhaContador++;
    }
    
    fclose(arquivo);
    fclose(arquivoTemp);

    remove(endereco);
    rename(enderecoTemp, endereco);
}

struct tm *gerarHorario(int hora,int minuto, int dia, int mes, int ano) {
    struct tm *horario = (struct tm*) malloc(sizeof(struct tm));

    horario->tm_hour = hora;
    horario->tm_min = minuto;
    horario->tm_sec = 0;
    horario->tm_mday = dia;
    horario->tm_mon = mes - 1; // Começa em 1
    horario->tm_year = ano - 1900; // Começa a contar de 1990
    horario->tm_isdst = -1; 

    return horario;
}
