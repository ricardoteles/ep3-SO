#ifndef FS_H
#define FS_H

#include <stdio.h>
#include <stdlib.h>

// #define TAM_FS				262144		/* tamanho do FS em bytes (= 256KB) */
// #define TAM_BLOCO			  4096		/* tamanho do bloco em bytes (= 4KB) */

#define TAM_FS				     16000		
#define TAM_BLOCO			  	 160		
#define TAM_FAT					 160	

FILE* arquivoMount;
int FAT[TAM_FAT];

int qtadeBlocos;
int iniSuperbloco;
int iniBitmap;		
int iniFat;		
int iniRaiz;
int iniDados;

void umountFS();
int mountFS(char* fname);
void escreveInt(int valor, int inicioBytes, int nbytes);
void escreveChar(char valor, int inicioBytes, int nbytes);
int leInt(int inicioBytes);

#endif