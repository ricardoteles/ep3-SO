#ifndef BINARIO_H
#define BINARIO_H

#include <stdio.h>
#include "dados.h"


void escreveInt(FILE* binario, int valor, int inicioBytes, int numInt);
void escreveChar(FILE* binario, char valor, int inicioBytes, int numChar);
void escreveStruct(FILE* binario, Arquivo item, int inicioBytes, int numStruct);
int leInt(FILE* binario, int inicioBytes);
char leChar(FILE* binario, int inicioBytes);
Arquivo leStruct(FILE* binario, int inicioBytes);

#endif