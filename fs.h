#ifndef FS_H
#define FS_H

#include <stdio.h>
#include "dados.h"

// #define TAM_FS				262144		/* tamanho do FS em bytes (= 256KB) */
// #define TAM_BLOCO			  4096		/* tamanho do bloco em bytes (= 4KB) */

#define TAM_FS				     16000		
#define TAM_BLOCO			  	 160		
#define NUM_BLOCOS				 (TAM_FS/TAM_BLOCO) 
#define TAM_FAT					 4*TAM_BLOCO

FILE* arquivo;

int umountFS();
int mountFS(char* fname);


#endif