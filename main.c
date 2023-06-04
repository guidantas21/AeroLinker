#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "include/aeroporto.h"
#include "include/conexao.h"
#include "include/caminho.h"
#include "include/voo.h"

#include "include/grafo.h"
#include "include/pilha.h"

#include "include/interface.h"

#include "include/debug.h"
#include "include/utils.h"

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

    cleanCMD();
    splashScreen(3);
    cleanCMD();

    int opcao;
    bool rodando = true;

    // int idInicial, idFinal;
    char iataInicial[4], iataFinal[4];

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP DA APLICAÇÃO ---------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (rodando) {
        printf("Menu\n");
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
                perguntaAeroporto(iataInicial, iataFinal);

                tAeroporto *aeroportoInicial = acharAeroportoPorIATA(stringMaiuscula(iataInicial), dadosAeroportos, numAeroportos);
                tAeroporto *aeroportoFinal = acharAeroportoPorIATA(stringMaiuscula(iataFinal), dadosAeroportos, numAeroportos);
            
                tCaminho *caminho = criaCaminho(aeroportos);

                menorDistancia(aeroportos, aeroportoInicial->id, aeroportoFinal->id, caminho);

                tVoo *voo = criarVoo(aeroportoInicial, aeroportoFinal, caminho);
                
                printVooInfo(voo);

                destruirVoo(voo);
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
    destruirConexoes(&dadosConexoes, &numConexoes);
    destruirAeroportos(&dadosAeroportos, &numAeroportos);
    // Desalocar memória alocada para armazenar o grafo 
    liberarGrafo(aeroportos);

    return 0;
}
