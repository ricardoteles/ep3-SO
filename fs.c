#include <stdio.h>
#include <stdlib.h>
#include "fs.h"

#define TAM_FS				262144		/* tamanho do FS em bytes (= 256KB) */
#define TAM_BLOCO			  4096		/* tamanho do bloco em bytes (= 4KB) */

#define INICIO_SUPERBLOCO 		0
#define TAM_SUPERBLOCO 			TAM_BLOCO

#define INICIO_BITMAP 			TAM_BLOCO
#define TAM_BITMAP 				TAM_BLOCO

#define INICIO_FAT				2*TAM_BLOCO
#define TAM_FAT					TAM_BLOCO

#define INICIO_DADOS			3*TAM_BLOCO
#define TAM_DADOS				61*TAM_BLOCO

static FILE* arquivo = NULL;

static int criaFS(char* fname); 
static int existeFS(char* fname);

void escreveFS(char valor, int inicio, int nbytes) {
	if (arquivo) {
		int i;
		char myVal = valor;

		fseek(arquivo, inicio * sizeof(char), SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&myVal, sizeof(char), 1, arquivo) != 1)
				printf("Não escrebeu 1 byte!\n");
	}
}

void imprimeFS() {
	if (arquivo) {
		char val;

		while (!feof(arquivo)) {
			if (fread(&val, sizeof(char), 1, arquivo) == 1) {
				printf("%d", val);
			}
		}
	}
}

int mountFS(char* fname) {
	// se o arquivo não foi aberto ainda
	if (!arquivo) {
		// se o arquivo não foi criado ainda
		if (!existeFS(fname) && !criaFS(fname)) { // criar pode dar errado se caminho for incoerente	
			return 0;
		}

		arquivo = fopen(fname, "r+b");
		if (arquivo) {
			printf("\n");
			return 1;
		}
	}

	return 0;
}

static int existeFS(char* fname) {
	// r+b eh modo que obriga existencia do arquivo binario
	arquivo = fopen(fname, "r+b");

	if (!arquivo) { // se NULL é porque não existe
		return 0;
	}

	fclose(arquivo);
	return 1;
}

static int criaFS(char* fname) {
	arquivo = fopen(fname, "wb");

	// ve se criou
	if (arquivo) {
		escreveFS(9, INICIO_SUPERBLOCO, TAM_SUPERBLOCO);			// escreve superbloco
		escreveFS(0, INICIO_BITMAP, TAM_BITMAP);					// escreve bitmap
		escreveFS(-2, INICIO_FAT, TAM_FAT);							// escreve FAT
		escreveFS(3, INICIO_DADOS, TAM_DADOS);						// escreve FAT
		
		fclose(arquivo);

		return 1;
	}
	return 0;
}