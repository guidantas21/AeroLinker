#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
    #define COMANDO_MAPA_PYTHON "python mapas/mapas.py"
    // const char* comando = "python arquivo.py";  // Comando para Windows
#else
    // const char* comando = "python3 arquivo.py"; // Comando para Linux e macOS
    #define COMANDO_MAPA_PYTHON "python3 mapas/mapas.py"
#endif

// Endereco para dados
#define AEROPORTOS_FILE "dados/aeroportos.csv"
// #define AEROPORTOS_FILE "C:\\dados\\aeroportos.csv"
#define CONEXOES_FILE "dados/conexoes.csv"
// #define CONEXOES_FILE "C:\\dados\\conexoes.csv"
// Config
#define DEBUG true

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEBUG ------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void debugAbrirArquivo(char endereco[], char modo[]) {
    if (DEBUG) printf("\nAbrindo '%s' no modo '%s'\n", endereco, modo);
}

void debugFecharArquivo(char endereco[]) {
    if (DEBUG) printf("\nFechando o arquivo '%s'\n", endereco);
}

void debugIniciarArquivo(char endereco[]) {
    if (DEBUG) printf("Arquivo '%s' iniciado com sucesso\n", endereco);
}

bool verificarArquivo(FILE *fptr, char endereco[]) {
    if (fptr == NULL) {
        if (DEBUG) printf("Erro ao abrir o arquivo '%s'\n", endereco);
        return false;
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MANIPULAÇÂO DE DADOS ---------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void debugAcharAeroporto(tAeroporto aeroporto, char iata[]) {
    if (DEBUG) {
        printf("Aeroporto de IATA '%s' encontrado: ", iata);
        printf("{ iata: %s, nome: %s, local: %s, pais: %s, latitude: %f, longitude: %f }\n", 
            aeroporto.iata, 
            aeroporto.nome, 
            aeroporto.local, 
            aeroporto.pais,
            aeroporto.latitude,
            aeroporto.longitude
        );
    }
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

tAeroporto *lerDadosAeroportos(unsigned int *numAeroportos) {
    debugAbrirArquivo(AEROPORTOS_FILE, "r");

    FILE *fptr = fopen(AEROPORTOS_FILE, "r");

    if (!verificarArquivo(fptr, AEROPORTOS_FILE)) return NULL;

    // Quantidade de aeroportos listados no arquivo
    *numAeroportos = contarLinhasDeArquivo(fptr);

    tAeroporto *aeroportos = (tAeroporto*) malloc((*numAeroportos) * sizeof(tAeroporto));

    if (aeroportos == NULL) {
        if (DEBUG) printf("Erro ao alocar memória para o vetor de aeroporto\n");
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

    debugFecharArquivo(AEROPORTOS_FILE);
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

tAeroporto *acharAeroportoPorIATA(char iata[4], tAeroporto *aeroportos, unsigned numAeroportos) {
    for (int i = 0; i < numAeroportos; i++) {
        if (!strcmp(aeroportos[i].iata, iata) ) {
            debugAcharAeroporto(aeroportos[i], iata);
            return &aeroportos[i];
        }
    }
    if (DEBUG) printf("Aeroporto de IATA '%s' não encontrado\n", iata);
    return NULL;
}

int idAerportoPorIATA(char iata[4], tAeroporto *aeroportos, unsigned numAeroportos) {
    for (int i = 0; i < numAeroportos; i++) {
        if (!strcmp(aeroportos[i].iata, iata) ) {
            debugAcharAeroporto(aeroportos[i], iata);
            if (DEBUG) printf("ID: %d\n", i);
            return i;
        }
    }
    if (DEBUG) printf("Aeroporto de IATA '%s' não encontrado\n", iata);
    return -1;
}

tConexao *lerDadosConexoes(tAeroporto *aeroportos, unsigned int numAeroportos, unsigned int *numConexoes) {
    debugAbrirArquivo(CONEXOES_FILE, "r");

    FILE *fptr = fopen(CONEXOES_FILE, "r");

    if (!verificarArquivo(fptr, CONEXOES_FILE)) return NULL;

    *numConexoes = contarLinhasDeArquivo(fptr);

    tConexao *conexoes = (tConexao*) malloc((*numConexoes) * sizeof(tConexao));

    if (conexoes == NULL) {
        if (DEBUG) printf("Erro ao alocar memória para o vetor de conexões\n");
        return NULL;
    }

    int index = 0;
    char iataInicial[4];
    char iataFinal[4];
    unsigned int distanciaKm;

    while (fscanf(fptr,"%[^,],%[^,],%d\n", iataInicial, iataFinal, &distanciaKm) == 3) {
        conexoes[index].inicial = acharAeroportoPorIATA(iataInicial, aeroportos, numAeroportos);

        if (conexoes[index].inicial == NULL) {
            if (DEBUG) printf("Aeroporto inicial inexistente\n");
            return NULL;
        }

        conexoes[index].final = acharAeroportoPorIATA(iataFinal, aeroportos, numAeroportos);

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
    debugFecharArquivo(CONEXOES_FILE);

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






///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GRAFOS -----------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    int distancia; // Distância entre dois vértices
} tAresta;

typedef struct {
    tAresta **arestas; // Matriz adjacente representando as arestas
    int numVertices; // Número de vértices no grafo
} tGrafo;

// Libera toda a memória alocada para o grafo
void liberarGrafo(tGrafo *grafo) {
    // Verifica se a matriz adjacente precisa ser liberada
    if (grafo->arestas != NULL) {
        for (int i = 0; i < grafo->numVertices; i++) {
            // Libera memória de cada vetor da matriz adjacente que não seja nulo
            if (grafo->arestas[i] != NULL) {
                free(grafo->arestas[i]);
                grafo->arestas[i] = NULL;
            }
        }
        // Libera memória da matriz adjacente
        free(grafo->arestas);
        grafo->arestas = NULL;
    }
    // Libera memória do grafo
    free(grafo);
    grafo = NULL;
}

// Cria um novo grafo com o número especificado de vértices
tGrafo *criarGrafo(int numVertices) {
    assert(numVertices > 0); // Verifica se o número de vértices é maior que 0
    
    // Aloca memória para um novo grafo
    tGrafo *novoGrafo = (tGrafo*) malloc(sizeof(tGrafo));

    // Verifica alocação do novo grafo
    if (novoGrafo == NULL) {
        return NULL;
    }

    novoGrafo->numVertices = numVertices;
    // Aloca memória para uma matriz adjacente de n vetores de arestas
    novoGrafo->arestas = (tAresta**) malloc(numVertices * sizeof(tAresta*));

    // Verifica alocação da matriz adjacente
    if (novoGrafo->arestas == NULL) {
        free(novoGrafo);
        novoGrafo = NULL;
        return NULL;
    }

    for (int i = 0; i < numVertices; i++) {
        // Aloca memória para cada vetor de n arestas da matriz 
        novoGrafo->arestas[i] = (tAresta*) malloc(numVertices * sizeof(tAresta));
        // Inicializa a matriz adjacente com distâncias iniciais
        for (int j = 0; j < numVertices; j++) {
            // Se o vertice inicial for o mesmo do final, a distância da aresta entre eles é 0
            if (i == j) {
                novoGrafo->arestas[i][j].distancia = 0; // Distância entre vértices iguais é zero
            } else {
                novoGrafo->arestas[i][j].distancia = -1; // Inicializa com distância -1 (sem aresta)
            }
            // Verifica alocação de cada vetor de arestas da matriz adjacente
            if (novoGrafo->arestas[i] == NULL) {
                liberarGrafo(novoGrafo);
                return NULL;
            }
        }
    }

    return novoGrafo;
}

// Verifica se uma aresta existe entre dois vértices no grafo
bool existeAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino) {
    // Verifica se os vértices de origem e destino da aresta são válidos
    assert(verticeOrigem >= 0 && verticeOrigem < grafo->numVertices);
    assert(verticeDestino >= 0 && verticeDestino < grafo->numVertices);

    // Verifica se a distância da aresta entre os vértices é diferente de -1
    return (grafo->arestas[verticeOrigem][verticeDestino].distancia != -1);
}

// Adiciona uma aresta ao grafo com a distância especificada
bool adicionarAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino, int distancia) {
    // Verifica se os vértices de origem e destino da aresta e distância da aresta são válidos
    assert(verticeOrigem >= 0 && verticeOrigem < grafo->numVertices);
    assert(verticeDestino >= 0 && verticeDestino < grafo->numVertices);
    assert(distancia >= 0);

    // Verifica se a aresta já existe, nesse caso a aresta não é adicionada
    if (existeAresta(grafo, verticeOrigem, verticeDestino)) {
        return false;
    }

    // Atualiza a distância da aresta no grafo
    grafo->arestas[verticeOrigem][verticeDestino].distancia = distancia;

    return true;
}

// Remove uma aresta do grafo
bool removerAresta(tGrafo *grafo, int verticeOrigem, int verticeDestino) {
    // Verifica se os vértices de origem e destino da aresta são válidos
    assert(verticeOrigem >= 0 && verticeOrigem < grafo->numVertices);
    assert(verticeDestino >= 0 && verticeDestino < grafo->numVertices);

    // Verifica se a aresta não existe, nesse caso a remoção não é possível
    if (!existeAresta(grafo, verticeOrigem, verticeDestino)) {
        return false;
    }

    // Atualiza a distância da aresta no grafo para -1, indicando que a aresta não existe
    grafo->arestas[verticeOrigem][verticeDestino].distancia = -1;

    return true;
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

void passarConexoesParaGrafo(tGrafo *grafo, tConexao *dadosConexoes, int numConexoes, tAeroporto *dadosAeroportos, int numAeroportos) {
    int inicio, destino;
    bool check;
    for (int i = 0; i < numConexoes; i++) {
        inicio = idAerportoPorIATA(dadosConexoes[i].inicial->iata, dadosAeroportos, numAeroportos);
        destino = idAerportoPorIATA(dadosConexoes[i].final->iata, dadosAeroportos, numAeroportos);
        check = adicionarAresta(grafo, inicio, destino, dadosConexoes[i].distanciaKm);

        if (DEBUG) {
            if (check) {
                printf("Conexão adicionada no grafo\n\n");
            } else {
                printf("Conexão inválida!\n\n");
            }
        }
    }
}

void mostrarMapaRedeAerea() {
    if (getenv("WSL_DISTRO_NAME") == NULL) {
        system(COMANDO_MAPA_PYTHON);
    } else {
        printf("Está sendo executado no Windows Subsystem for Linux (WSL).\n");
    }
} 

void print_logo(char endereco[]){
    FILE *fptr = fopen(endereco,"r");

    if(fptr == NULL){
        // fprintf(stderr,"error opening %s\n",logo);
        return;
    }

    char read_string[128];
 
    while(fgets(read_string,sizeof(read_string),fptr) != NULL)
    printf("%s",read_string);

    fclose(fptr);
}

int main() {
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SETUP ---------------------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Variáveis para aramzenar dados dos aeroportos e conexões
    tAeroporto *dadosAeroportos;
    unsigned int numAeroportos = 0;

    tConexao *dadosConexoes;
    unsigned int numConexoes = 0;

    // Verifica arquivos de dados de aeroportos e conexões
    iniciarArquivo(AEROPORTOS_FILE);
    iniciarArquivo(CONEXOES_FILE);

    // Gerar lista de aeroportos e conexões a partir de dados dos arquivos
    dadosAeroportos = lerDadosAeroportos(&numAeroportos);
    dadosConexoes = lerDadosConexoes(dadosAeroportos, numAeroportos, &numConexoes);

    // Criar grafo vazio
    tGrafo *aeroportos = criarGrafo(numAeroportos);

    // Passar dados pra o grafo
    passarConexoesParaGrafo(aeroportos, dadosConexoes, numConexoes, dadosAeroportos, numAeroportos);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP DA APLICAÇÃO ---------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    /*
    TODO:
    criar loop infinito
    mostrar opcoes
        1. printar dados dos aeroportos
        2. printar dados das conexões
        3. printar arestas
        4. Mostrar mapa da rede aérea
        0. Sair do prgrama (finaliza o loop da aplicação)
    perguntar o que o usário quer
    fazer o que o usário pedir (executar umas das funçoes disponíveis ou sair do programa)
    */

    char *logo = "dados/logo.txt";
 
    print_logo(logo);

    int opcao;

    printf("menu\n");
    printf("1. dados dos aeroportos\n");
    printf("2. dados das conexões\n");
    printf("3. printar arestas\n");
    printf("4. mapa da rede aérea\n");
    printf("5. cadastrar voos\n");
    printf("6. remover voos\n");
    printf("0. Sair do programa\n\n");
    scanf ("%d", &opcao);

    switch (opcao){
    case 1: // Print tabelas de dados de aeroportos
        printAeroportos(dadosAeroportos, numAeroportos);
        break;

    case 2: // Print tabelas de dados de conexões
        printConexoes(dadosConexoes, numConexoes);
        break;

    case 3: // Print matriz adjacente do grafo
        printArestas(aeroportos, numAeroportos);
        break;
    
    case 4: // Mostrar mapa da rede aérea no navegador
        mostrarMapaRedeAerea();
        break;
    
    case 5: // Adicionar voos
        printf ("\nFunção 5 ainda não adicionada\n\n");
        break;

    case 6: // Remover voos
        printf ("\nFunção 6 ainda não adicionada\n\n");
        break;

    case 0: // Encerrar loop
        printf ("Encerrar aplicação");
        break;

    default:
        printf("Selecione uma opção válida");
        break;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LIBERAÇÃO DE MEMÓRIA ------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Desalocar memória alocada para armazenar dados de coexões e aeroportos
    destruirConexoes(&dadosConexoes, &numConexoes);
    destruirAeroportos(&dadosAeroportos, &numAeroportos);
    // Desalocar memória alocada para armazenar o grafo 
    liberarGrafo(aeroportos);

    return 0;
}
