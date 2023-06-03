#include <stdio.h>
#include <stdbool.h>

#include "../include/debug.h"

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