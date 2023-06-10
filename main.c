#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "include/aeroporto.h"
#include "include/conexao.h"
#include "include/caminho.h"
#include "include/voo.h"
#include "include/dados.h"

#include "include/grafo.h"
#include "include/pilha.h"

#include "include/interface.h"

#include "include/debug.h"
#include "include/utils.h"

int main() {
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SETUP ---------------------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Verifica arquivos de dados
    iniciarArquivo(AEROPORTOS_FILE);
    iniciarArquivo(CONEXOES_FILE);
    iniciarArquivo(VOOS_FILE);

    // Iniciar dados
    tDados dados = {
        .numAeroportos = 0, 
        .numConexoes = 0, 
        .numVoos = 0 
    };

    dados.aeroportos = lerDadosAeroportos(&dados.numAeroportos);
    dados.conexoes = lerDadosConexoes(dados.aeroportos, dados.numAeroportos, &dados.numConexoes);
    dados.voos = lerDadosVoos(dados.aeroportos, dados.numAeroportos, &dados.numVoos);
    

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
            case '1': // Print tabelas de dados de aeroportos
                cleanCMD();
                exibirDados(dados, redeAeroportos);
                break;
            
            case '2': // Mostrar mapa da rede aérea no navegador
                cleanCMD();
                menuMapas();
                break;
            
            case '3': // Adicionar voos
            {
                cleanCMD();

                char iataInicial[4];
                char iataFinal[4];
                
                // Input IATA do aeroporto inicial e final
                perguntaAeroporto(iataInicial, iataFinal);

                // Bucar aeroporto inicial e final por IATA
                tAeroporto *aeroportoInicial = acharAeroportoPorIATA(
                    stringMaiuscula(iataInicial), 
                    dados.aeroportos, 
                    dados.numAeroportos
                );

                tAeroporto *aeroportoFinal = acharAeroportoPorIATA(
                    stringMaiuscula(iataFinal), 
                    dados.aeroportos, 
                    dados.numAeroportos
                );


                // Gerar menor trajeto entre aeroporto inicial e final
                tCaminho *trajeto = criaCaminho(redeAeroportos->numVertices);
            
                menorDistancia(redeAeroportos, aeroportoInicial->id, aeroportoFinal->id, trajeto);
                
                // Abrir espaço na lista de voos
                dados.numVoos++;
            
                dados.voos = realloc(dados.voos, dados.numVoos * sizeof(tVoo*));


                // Gerar hórario de saída do voo
                int hora, minuto, dia, mes, ano;

                inputData(&dia, &mes, &ano);
                inputHorario(&hora, &minuto);

                struct tm *horarioSaida = gerarHorario(hora, minuto, dia, mes, ano);
                

                // Criar o voo, adicionar na lista de voos e salvar no arquivo
                int ultimoIndex = dados.numVoos-1;

                dados.voos[ultimoIndex] = criarVoo(trajeto, aeroportoInicial, aeroportoFinal, horarioSaida, ultimoIndex);

                salvarVoo(dados.voos[ultimoIndex], dados.aeroportos, dados.numAeroportos);

                break;
            }
            case '4':
            {
                cleanCMD();
                pesquisarVoo(dados.voos,&dados.numVoos,dados.aeroportos,dados.numAeroportos);
                break;
            }
            case '0': // Encerrar loop
                printf("\nEncerrando aplicação\n");
                rodando = false;
                break;

            default:
                printf("\nSelecione uma opção válida\n");
                break;
        }
        cleanCMD();
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LIBERAÇÃO DE MEMÓRIA ------------------------------------------------------------------------------------
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Liberar memória alocada para armazenar dados de coexões e aeroportos
    destruirConexoes(&dados.conexoes, &dados.numConexoes);
    destruirAeroportos(&dados.aeroportos, &dados.numAeroportos);

    // Liberar memória do grafo de redes aéreas
    liberarGrafo(redeAeroportos);

    // Liberar memória dos voos
    for (int i = 0; i < dados.numVoos; i++)
        destruirVoo(dados.voos[i]);

    return 0;
}
