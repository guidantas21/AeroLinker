#pragma once

#include <stdbool.h>

#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef _WIN32
    #define COMANDO_MAPA_PYTHON "python mapas/mapas.py"
    #define CLEAR_SCREEN_COMMAND "cls"
#else
    #define COMANDO_MAPA_PYTHON "python3 mapas/mapas.py"
    #define CLEAR_SCREEN_COMMAND "clear"
#endif

#define TEMPO_SPLASH_SCREEN 3
#define CLEAN_CMD false

void mostrarMapaRedeAerea();

void print_logo(char endereco[]);

void splashScreen(int duracao);

void perguntaAeroporto(char iataInicial[], char iataFinal[]);

void cleanCMD();

void printMenu();

int inputOpcao();

#endif