#ifndef FS_H
#define FS_H

#include <stdio.h>
#include "dados.h"

// #define TAM_FS				262144		/* tamanho do FS em bytes (= 256KB) */
// #define TAM_BLOCO			  4096		/* tamanho do bloco em bytes (= 4KB) */

#define TAM_FS				     16000		
#define TAM_BLOCO			  	 160		
#define TAM_FAT					 4*TAM_BLOCO  // em bytes!

FILE* arquivo;
Arquivo raiz;

int qtadeBlocos;
int qtadeBlocosLivres;
int memUsada;

int iniSuperbloco;
int iniBitmap;
int iniFat;
int FAT[TAM_FAT];

int qteDiretorios;
int qteArquivos;
int espacoLivre;
int espacoDesperd;


int changeFAT;

void criaFS(char* fname); 
int existeFS(char* fname);
void carregaFS();
int umountFS();
int mountFS(char* fname);

#endif