#include "fs.h"
#include "fat.h"

void criaFAT() {
	escreveInt(-2, iniFat, iniRaiz-iniFat);				// inicializa o FAT com -2
}

void carregaFATnaMemoria(int inicioBytes, int qtdeBlocosDados) {
	int i;

	if (arquivoMount) {
		int val;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < qtdeBlocosDados; i++) {
			if (fread(&val, sizeof(char), 4, arquivoMount) == 4) {
				FAT[i] = val;
			}
		}
	}
}

void regravaFATnoDisco(int inicioBytes, int nbytes) {
	if (arquivoMount) {
		int i;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&FAT[i], sizeof(int), 1, arquivoMount) != 1)
				printf("Não escreveu 1 int!\n");
	}
}