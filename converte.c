#include <stdio.h>
#include <stdlib.h>

#define TAM_BLOCO			  	 160

static FILE* arquivo = NULL;

int FAT[1000];

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
			if (fread(&val, sizeof(int), 1, arquivo) == 1) {
				printf("%d|", val);
				i++;
			}
		}
	}
	printf("|");
}

int main(int argc, char* argv[]) {
	arquivo = fopen(argv[1], "r+b");

	printf("\n\nSUPERBLOCO\n\n");
	imprimeInt(0, 6);		//superbloco

	printf("\n\nBITMAP\n\n");
	imprimeChar(160, 160); 	//bitmap

	printf("\n\nFAT\n\n");
	imprimeInt(320, 160); 	//FAT

	printf("\n\nRAIZ\n\n");
	imprimeInt(960, 1);		//Raiz
	imprimeChar(961, 159);	//Raiz

	return 0;
}