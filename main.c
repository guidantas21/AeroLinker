#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Endereco para dados
#define AEROPORTOS_FILE "dados/aeroportos.txt"
#define CONEXOES_FILE "dados/conexoes.txt"
// Config
#define DEBUG true

typedef struct {
    char sigla[4];
    char nome[50];
    char cidade[50];
    char pais[50];
} tAeroporto;

typedef struct {
    tAeroporto inicial;
    tAeroporto final;
    unsigned int distanciaKM;
} tConexao;

bool iniciarArquivo(char endereco[]) {
    if (DEBUG) printf("Abrindo '%s'\n", endereco);

    // Se o arquivo não existir, será criado
    FILE *fptr = fopen(endereco, "a");

    if (fptr == NULL) {
        if (DEBUG) printf("Erro ao abrir o arquivo '%s'\n", endereco);
        return false;
    }

    if (DEBUG) printf("Arquivo '%s' aberto com sucesso\n", endereco);

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

tAeroporto *lerDadosAeroportos(unsigned int *numAeroportos) {
    if (DEBUG) printf("Abrindo '%s' para leitura\n", AEROPORTOS_FILE);

    FILE *fptr = fopen(AEROPORTOS_FILE, "r");

    if (fptr == NULL) {
        if (DEBUG) printf("Erro ao abrir o arquivo '%s'\n", AEROPORTOS_FILE);
        return NULL;
    }

    // Quantidade de aeroportos listados no arquivo
    *numAeroportos = contarLinhasDeArquivo(fptr);

    tAeroporto *aeroportos = (tAeroporto*) malloc((*numAeroportos) * sizeof(tAeroporto));

    if (aeroportos == NULL) {
        if (DEBUG) printf("Erro ao alocar memória para o vetor de aeroportos '%s'\n", AEROPORTOS_FILE);
        return NULL;
    }

    // Pega as informações de cada aeroporto e cria um tAeroporto com elas
    int index = 0;
    while (fscanf(fptr,"%[^,],%[^,],%[^,],%[^\n]\n", 
        aeroportos[index].sigla, 
        aeroportos[index].nome, 
        aeroportos[index].cidade, 
        aeroportos[index].pais) == 4) {
        index++;
    }

    fclose(fptr);

    return aeroportos;
}

void printAeroportos(tAeroporto *aeroportos, unsigned int numAeroportos) {
    for (int i = 0; i < numAeroportos; i++) {
        printf("|  %3s\t | \t", aeroportos[i].sigla);
        printf("%-*s\t | \t",  25, aeroportos[i].nome);
        printf("%-*s\t | \t", 25, aeroportos[i].cidade);
        printf("%-*s\n", 25, aeroportos[i].pais);
    }
}

void destruirAeroportos(tAeroporto *aeroportos, unsigned int *numAeroportos) {
    if (DEBUG) printf("Liberando memória alocada do vetor de aeroportos\n");

    // Libera memória locada para o vetor
    *numAeroportos = 0;
    free(aeroportos);
    aeroportos = NULL;
}

tAeroporto acharAeroportoPorSigla(char sigla[4], tAeroporto *aeroportos, unsigned numAeroportos) {
    for (int i = 0; i < numAeroportos; i++) {
        if (!strcmp(aeroportos[i].sigla, sigla)) {
            if (DEBUG) printf("Aeroporto de sigla '%s' encontrado\n", sigla);
            return aeroportos[i];
        }
    }
    if (DEBUG) printf("Aeroporto de sigla '%s' não encontrado\n", sigla);
}

tConexao *lerDadosConexoes(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numConexoes) {
    if (DEBUG) printf("Abrindo '%s' para leitura\n", CONEXOES_FILE);

    FILE *fptr = fopen(CONEXOES_FILE, "r");

    if (fptr == NULL) {
        if (DEBUG) printf("Erro ao abrir o arquivo '%s'\n", CONEXOES_FILE);
        return NULL;
    }

    *numConexoes = contarLinhasDeArquivo(fptr);

    tConexao *conexoes = (tConexao*) malloc((*numConexoes) * sizeof(tConexao));

    if (conexoes == NULL) {
        if (DEBUG) printf("Erro ao alocar memória para o vetor de conexões '%s'\n", CONEXOES_FILE);
        return NULL;
    }

    int index = 0;
    char siglaInicial[4];
    char siglaFinal[4];
    unsigned int distanciaKM;

    while (fscanf(fptr,"%[^,],%[^,],%d\n", siglaInicial, siglaFinal, &distanciaKM) == 3) {
        conexoes[index].inicial = acharAeroportoPorSigla(siglaInicial, aeroportos, numAeroportos);
        conexoes[index].final = acharAeroportoPorSigla(siglaFinal, aeroportos, numAeroportos);
        conexoes[index].distanciaKM = distanciaKM;
        index++;
    }

    fclose(fptr);

    return conexoes;
}

void printConexoes(tConexao *conexoes, unsigned int numConexoes) {
    for (int i = 0; i < numConexoes; i++) {
        printf("|  %3s\t | \t", conexoes[i].inicial.sigla);
        printf("|  %3s\t | \t", conexoes[i].final.sigla);
        printf("%-*d |\n", 10, conexoes[i].distanciaKM);
    }
}

void destruirConexoes(tConexao *conexoes, unsigned int *numConexoes) {
    if (DEBUG) printf("Liberando memória alocada do vetor de conexões\n");

    free(conexoes);
    *numConexoes = 0;
    conexoes = NULL;
}

int main() {
    tAeroporto *aeroportos;
    unsigned int numAeroportos = 0;

    tConexao *conexoes;
    unsigned int numConexoes = 0;

    iniciarArquivo(AEROPORTOS_FILE);
    iniciarArquivo(CONEXOES_FILE);

    aeroportos = lerDadosAeroportos(&numAeroportos);
    conexoes = lerDadosConexoes(aeroportos, numAeroportos, &numConexoes);

    printf("\nAeroportos cadastrados:\n");
    printAeroportos(aeroportos, numAeroportos);
    printf("\nConexoes cadastradas:\n");
    printConexoes(conexoes, numConexoes);

    destruirAeroportos(aeroportos, &numAeroportos);
    destruirConexoes(conexoes, &numConexoes);

    return 0;
}
