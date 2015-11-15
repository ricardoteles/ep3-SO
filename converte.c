#include <stdio.h>
#include <stdlib.h>

#define TAM_BLOCO			  	 16

static FILE* arquivo = NULL;

void imprimeChar(int inicioBytes, int quantidadeBlocos) {
	int i = 0;

	if (arquivo) {
		char val;

		fseek(arquivo, inicioBytes, SEEK_SET);

		while (i < quantidadeBlocos) {
			if (fread(&val, sizeof(char), 1, arquivo) == 1) {
				printf("%d|", val);
				i++;
			}
		}
	}
	printf("|");
}

void imprimeInt(int inicioBytes, int quantidadeBlocos) {
	int i = 0;

	if (arquivo) {
		int val;

		fseek(arquivo, inicioBytes, SEEK_SET);

		while (i < quantidadeBlocos) {
			if (fread(&val, sizeof(char), 4, arquivo) == 4) {
				printf("%d|", val);
				i++;
			}
		}
	}
	printf("|");
}

int main(int argc, char* argv[]) {
	arquivo = fopen(argv[1], "r+b");

	imprimeInt(0, 4);
	imprimeChar(TAM_BLOCO, TAM_BLOCO);
	imprimeInt(2*TAM_BLOCO, TAM_BLOCO);

	return 0;
}