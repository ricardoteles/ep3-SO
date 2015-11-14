#include <stdio.h>
#include <stdlib.h>
#include "fs.h"

#define INICIO_GERESPLIV 	10
#define INICIO_FAT			20

static FILE* arquivo = NULL;

static int criaFS(char* fname); 
static int existeFS(char* fname);

void escreveFS(char valor, int inicio, int nbytes) {
	if (arquivo) {
		int i;
		char myVal = valor;
		//arquivo = fopen(fname, "r+b");
		fseek(arquivo, inicio * sizeof(char), SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&myVal, sizeof(char), 1, arquivo) != 1)
				printf("Não escrebeu 1 byte!\n");
		//fclose(arquivo);
	}
}

void imprimeFS() {
	if (arquivo) {
		char val;
		//arquivo = fopen(fname, "rb");

		while (!feof(arquivo)) {
			if (fread(&val, sizeof(char), 1, arquivo) == 1) {
				printf("%d", val);
			}
		}
		//fclose(arquivo);
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
			imprimeFS();
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
		escreveFS(3,0,10);						// escreve superbloco
		escreveFS(0,INICIO_GERESPLIV,10);		// escreve gerenciamentoEspacoLivre
		escreveFS(2,INICIO_FAT,10);			// escreve FAT

		fclose(arquivo);

		return 1;
	}
	return 0;
}