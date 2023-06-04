#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "include/aeroporto.h"
#include "include/conexao.h"
#include "include/caminho.h"
#include "include/voo.h"

#include "include/grafo.h"
#include "include/pilha.h"

#include "include/interface.h"

#include "include/debug.h"
#include "include/utils.h"

typedef struct {
    tAeroporto *aeroportos;
    unsigned int numAeroportos;
    tConexao *conexoes;
    unsigned int numConexoes;
    tVoo *voos[100];
    unsigned int numVoos;
} tDados;

int main() {
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SETUP ---------------------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Verifica arquivos de dados
    iniciarArquivo(AEROPORTOS_FILE);
    iniciarArquivo(CONEXOES_FILE);
    iniciarArquivo(VOOS_FILE);

    tDados dados = {
        .numAeroportos = 0,
        .numConexoes = 0,
        .numVoos = 0
    };

    dados.aeroportos = lerDadosAeroportos(&dados.numAeroportos);
    dados.conexoes = lerDadosConexoes(dados.aeroportos, dados.numAeroportos, &dados.numConexoes);

    // Criar grafo vazio
    tGrafo *redeAeroportos = criarGrafo(dados.numAeroportos);

    // Passar dados pra o grafo
    passarConexoesParaGrafo(redeAeroportos, dados.conexoes, dados.numConexoes, dados.aeroportos, dados.numAeroportos);

    cleanCMD();
    splashScreen(TEMPO_SPLASH_SCREEN);
    cleanCMD();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP DA APLICAÇÃO ---------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool rodando = true;

    while (rodando) {
        printMenu();
        switch (inputOpcao()){
            case 1: // Print tabelas de dados de aeroportos
                printAeroportos(dados.aeroportos, dados.numAeroportos);
                break;

            case 2: // Print tabelas de dados de conexões
                printConexoes(dados.conexoes, dados.numConexoes);
                break;

            case 3: // Print matriz adjacente do grafo
                printArestas(redeAeroportos, dados.numAeroportos);
                break;
            
            case 4: // Mostrar mapa da rede aérea no navegador
                mostrarMapaRedeAerea();
                break;
            
            case 5: // Adicionar voos
                // Input IATA do aeroporto inicial e final
                char *iataInicial = (char*) malloc(4 * sizeof(char));
                char *iataFinal = (char*) malloc(4 * sizeof(char));

                perguntaAeroporto(iataInicial, iataFinal);

                // Bucar aeroporto inicial e final por IATA
                tAeroporto *aeroportoInicial = acharAeroportoPorIATA(stringMaiuscula(iataInicial), dados.aeroportos, dados.numAeroportos);
                tAeroporto *aeroportoFinal = acharAeroportoPorIATA(stringMaiuscula(iataFinal), dados.aeroportos, dados.numAeroportos);

                // Gerar menor trajeto entre aeroporto inicial e final
                tCaminho *caminho = criaCaminho(redeAeroportos);
                menorDistancia(redeAeroportos, aeroportoInicial->id, aeroportoFinal->id, caminho);

                // Inseriraeroportos e trajeto na lista voos
                dados.voos[dados.numVoos] = criarVoo(aeroportoInicial, aeroportoFinal, caminho);
                dados.numVoos++;
                
                // (TEMPORÁRIO) print voos da lista de voos
                for (int i = 0; i < dados.numVoos; i++)
                    printVooInfo(dados.voos[i], dados.aeroportos, dados.numAeroportos);

                // Liberar memória
                free(iataInicial);
                iataInicial = NULL;
                free(iataFinal);
                iataFinal = NULL;

                break;

            case 6: // Remover voos
                printf ("\nFunção 6 ainda não adicionada\n\n");
                break;

            case 0: // Encerrar loop
                printf ("\nEncerrando aplicação\n");
                rodando = false;
                break;

            default:
                printf("\nSelecione uma opção válida\n");
                break;
        }
    }
    

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LIBERAÇÃO DE MEMÓRIA ------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Desalocar memória alocada para armazenar dados de coexões e aeroportos
    destruirConexoes(&dados.conexoes, &dados.numConexoes);
    destruirAeroportos(&dados.aeroportos, &dados.numAeroportos);
    liberarGrafo(redeAeroportos);

    for (int i = 0; i < dados.numVoos; i++)
        destruirVoo(dados.voos[i]);

    return 0;
}
