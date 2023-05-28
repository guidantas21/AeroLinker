#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Endereco para dados
#define AEROPORTOS_FILE "dados/aeroportos.csv"
#define CONEXOES_FILE "dados/conexoes.csv"
// Config
#define DEBUG true

typedef struct {
    char iata[4];
    char nome[50];
    char local[50];
    char pais[50];
    float latitude;
    float longitude;
} tAeroporto;

typedef struct {
    tAeroporto *inicial;
    tAeroporto *final;
    unsigned int distanciaKm;
} tConexao;

bool iniciarArquivo(char endereco[]) {
    if (DEBUG) printf("Abrindo '%s' para gravação\n", endereco);

    // Se o arquivo não existir, será criado
    FILE *fptr = fopen(endereco, "a");

    if (fptr == NULL) {
        if (DEBUG) printf("Erro ao abrir o arquivo '%s'\n", endereco);
        return false;
    }

    if (DEBUG) printf("Arquivo '%s' iniciado com sucesso\n", endereco);

    if (DEBUG) printf("Fechando o arquivo '%s'\n", endereco);
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
    while (fscanf(fptr,"%[^,],%[^,],%[^,],%[^,],%f,%f\n", 
        aeroportos[index].iata, 
        aeroportos[index].nome, 
        aeroportos[index].local, 
        aeroportos[index].pais,
        &aeroportos[index].latitude,
        &aeroportos[index].longitude) == 6) {
        index++;
    }

    if (DEBUG) printf("Fechando o arquivo '%s'\n", AEROPORTOS_FILE);
    fclose(fptr);

    return aeroportos;
}

void printAeroportos(tAeroporto *aeroportos, unsigned int numAeroportos) {
    printf("\n- Aeroportos cadastrados:\n");
    printf("| IATA  | AEROPORTO\t\t\t | LOCAL\t\t\t | PAÍS\t\t\t\t | LATITUDE\t| LONGITUDE    |\n");
    for (int i = 0; i < numAeroportos; i++) {
        printf("| %3s\t| ", aeroportos[i].iata);
        printf("%-*s\t | ",  25, aeroportos[i].nome);
        printf("%-*s\t | ", 25, aeroportos[i].local);
        printf("%-*s\t | ", 25, aeroportos[i].pais);
        printf("%-*f | ", 12, aeroportos[i].latitude);
        printf("%-*f |\n", 12, aeroportos[i].longitude);
    }
    printf("\n");
}

void destruirAeroportos(tAeroporto **aeroportos, unsigned int *numAeroportos) {
    if (DEBUG) printf("Liberando memória alocada do vetor de aeroportos\n");

    // Libera memória locada para o vetor
    free(*aeroportos);
    *numAeroportos = 0;
    *aeroportos = NULL;
}

tAeroporto *acharAeroportoPorIata(char iata[4], tAeroporto *aeroportos, unsigned numAeroportos) {
    for (int i = 0; i < numAeroportos; i++) {
        if (!strcmp(aeroportos[i].iata, iata) ) {
            if (DEBUG) {
                printf("Aeroporto de IATA '%s' encontrado: ", iata);
                printf("{ iata: %s, nome: %s, local: %s, pais: %s, latitude: %f, longitude: %f }\n", 
                    aeroportos[i].iata, 
                    aeroportos[i].nome, 
                    aeroportos[i].local, 
                    aeroportos[i].pais,
                    aeroportos[i].latitude,
                    aeroportos[i].longitude
                );
            }
            return &aeroportos[i];
        }
    }
    if (DEBUG) printf("Aeroporto de IATA '%s' não encontrado\n", iata);
    return NULL;
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
    char iataInicial[4];
    char iataFinal[4];
    unsigned int distanciaKm;

    while (fscanf(fptr,"%[^,],%[^,],%d\n", iataInicial, iataFinal, &distanciaKm) == 3) {
        conexoes[index].inicial = acharAeroportoPorIata(iataInicial, aeroportos, numAeroportos);

        if (conexoes[index].inicial == NULL) {
            if (DEBUG) printf("Aeroporto inicial inexistente\n");
            return NULL;
        }

        conexoes[index].final = acharAeroportoPorIata(iataFinal, aeroportos, numAeroportos);

        if (conexoes[index].final == NULL) {
            if (DEBUG) printf("Aeroporto final inexistente\n");
            return NULL;
        }

        conexoes[index].distanciaKm = distanciaKm;
        
        if (DEBUG) {
            printf("Conexão encontrada: ");
            printf("{ inicial.iata: %s, final.iata: %s, distanciaKm: %d }\n\n", 
                conexoes[index].inicial->iata, 
                conexoes[index].final->iata, 
                conexoes[index].distanciaKm
            );
        }
        index++;
    }

    fclose(fptr);
    if (DEBUG) printf("Fechando o arquivo '%s'\n", CONEXOES_FILE);

    return conexoes;
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

void destruirConexoes(tConexao **conexoes, unsigned int *numConexoes) {
    if (DEBUG) printf("Liberando memória alocada do vetor de conexões\n");

    free(*conexoes);
    *numConexoes = 0;
    *conexoes = NULL;
}

int main() {
    tAeroporto *aeroportos;
    unsigned int numAeroportos = 0;

    tConexao *conexoes;
    unsigned int numConexoes = 0;

    iniciarArquivo(AEROPORTOS_FILE);
    iniciarArquivo(CONEXOES_FILE);

    // Gerar lista de aeroportos e conexões a partir de dados dos arquivos
    aeroportos = lerDadosAeroportos(&numAeroportos);
    conexoes = lerDadosConexoes(aeroportos, numAeroportos, &numConexoes);

    // Print tabelas de dados de aeroportos e conexões
    printAeroportos(aeroportos, numAeroportos);
    printConexoes(conexoes, numConexoes);

    // Desalocar memória
    destruirConexoes(&conexoes, &numConexoes);
    destruirAeroportos(&aeroportos, &numAeroportos);

    return 0;
}
