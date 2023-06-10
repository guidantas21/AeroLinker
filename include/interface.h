#pragma once

#include <stdbool.h>

#include "../include/aeroporto.h"
#include "../include/conexao.h"
#include "../include/caminho.h"
#include "../include/voo.h"
#include "../include/grafo.h"

#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef _WIN32
    #define COMANDO_MAPA_PYTHON "python maps/mapas.py "
    #define CLEAR_SCREEN_COMMAND "cls"
#else
    #define COMANDO_MAPA_PYTHON "python3 maps/mapas.py "
    #define CLEAR_SCREEN_COMMAND "clear"
#endif

#define TEMPO_SPLASH_SCREEN 3
#define CLEAN_CMD false

void mostrarMapa(char *protcolo);

void mostrarMapaRedeAerea();

void mostrarMapaVoos();

void mostrarMapaVoo(char *pk);

void print_logo(char endereco[]);

void splashScreen(int duracao);

void perguntaAeroporto(char iataInicial[], char iataFinal[]);

void printAeroportos(tAeroporto *aeroportos, unsigned int numAeroportos);

void printConexoes(tConexao *conexoes, unsigned int numConexoes);

void printVooInfo(tVoo *voo, tAeroporto *aeroportos, int numAeroportos);

void printArestas(tGrafo *grafo, int numVertices);

void inputData(int *dia, int *mes, int *ano);

void inputHorario( int *hora, int* minuto);

void menuVoo(tVoo **voos, unsigned int *numVoos, unsigned int id);

void pesquisarVoo(tVoo **voos, unsigned int *numVoos, tAeroporto *aeroportos, unsigned int numAeroportos);

char *inputIdStr();

unsigned int inputId();

void cleanCMD();

void printMenu();

int inputOpcao();

#endif