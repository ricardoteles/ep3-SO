#include "fs.h"
#include "fat.h"

int FAT[1000];

void criaFAT() {
	escreveInt(-2, 2*TAM_BLOCO, 4*TAM_BLOCO);				// inicializa o FAT com -2
}

void iniciaFATnaMemoria(int inicioBytes, int quantidadeBlocos){
	int i;

	if (arquivoMount) {
		int val;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < quantidadeBlocos; i++) {
			if (fread(&val, sizeof(char), 4, arquivoMount) == 4) {
				FAT[i] = val;
			}
		}
	}
}

void escreveFATnoDisco(int valor, int inicioBytes, int nbytes) {
	if (arquivoMount) {
		int i;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&FAT[i], sizeof(int), 1, arquivoMount) != 1)
				printf("Não escreveu 1 int!\n");
	}
}