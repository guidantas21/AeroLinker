#ifndef DEBUG_H
#define DEBUG_H

#include "aeroporto.h"

#define DEBUG false

void debugAbrirArquivo(char endereco[], char modo[]);

void debugFecharArquivo(char endereco[]);

void debugIniciarArquivo(char endereco[]);

void debugAcharAeroporto(tAeroporto aeroporto, char iata[]);

#endif