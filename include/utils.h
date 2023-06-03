#ifndef UTILS_H
#define UTILS_H

bool verificarArquivo(FILE *fptr, char endereco[]);

bool iniciarArquivo(char endereco[]);

unsigned int contarLinhasDeArquivo(FILE *fptr);

#endif