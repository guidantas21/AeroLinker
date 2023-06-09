#pragma once

#include <time.h>

#include "../include/caminho.h"

#ifndef UTILS_H
#define UTILS_H

bool verificarArquivo(FILE *fptr, char endereco[]);

bool iniciarArquivo(char endereco[]);

unsigned int contarLinhasDeArquivo(FILE *fptr);

char *stringMaiuscula(char* str);

void removerLinhaDoAquivo(char endereco[], unsigned int linha);

struct tm *gerarHorario(int hora,int minuto, int dia, int mes, int ano);

#endif