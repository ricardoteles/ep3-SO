#ifndef FS_H
#define FS_H

#include <stdio.h>
#include <stdlib.h>

#define TAM_FS				     1600		
#define TAM_BLOCO			  	 16		

FILE* arquivoMount;

void escreveInt(int valor, int inicioBytes, int nbytes);
void escreveChar(char valor, int inicioBytes, int nbytes);
int mountFS(char* fname);

#endif