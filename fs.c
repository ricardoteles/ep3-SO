#include "fs.h"
#include <stdio.h>

static FILE* arquivo;

void escreveArquivoBinario(char* fname, char valor, int inicio, int nbytes) {
	int i;
	char myVal = valor;

	arquivo = fopen(fname, "r+b");  //o arquivo tem que existir!
	fseek(arquivo, inicio * sizeof(char), SEEK_SET);

	for (i = 0; i < nbytes; i++)
		if (fwrite (&myVal, sizeof(char), 1, arquivo) != 1)
			printf("Não escrebeu 1 byte!\n");

	fclose(arquivo);
}

void imprimeArquivoBinario(char* fname) {
	char val;
	arquivo = fopen(fname, "rb");

	while (!feof(arquivo)) {
		if (fread(&val, sizeof(char), 1, arquivo) == 1) {
			printf("%d ", val);
		}
	}

	fclose(arquivo);
}

FILE* existeFS(char* fname){
	arquivo = fopen(fname, "r+b");	

	return arquivo;
}

void inicializaArquivoBinario(char* fname) {
	arquivo = fopen(fname, "wb");
	fclose(arquivo);
}