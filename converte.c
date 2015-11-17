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
			if (fread(&val, sizeof(char), 4, arquivo) == 4) {
				printf("%d|", val);
				i++;
			}
		}
	}
	printf("|");
}

// void carregaFATnaMemoria(int inicioBytes, int quantidadeBlocos){
// 	int i;

// 	if (arquivo) {
// 		int val;

// 		fseek(arquivo, inicioBytes, SEEK_SET);

// 		for (i = 0; i < quantidadeBlocos; i++) {
// 			if (fread(&val, sizeof(char), 4, arquivo) == 4) {
// 				FAT[i] = val;
// 			}
// 		}
// 	}
// }

// void imprimeFAT(int quantidadeBlocos){
// 	int i;

// 	for(i = 0; i < quantidadeBlocos; i++){
// 		if(i < 10) {
// 			if(FAT[i] > 0 && FAT[i] < 10) printf("  %d  [   %d  ]\n", i, FAT[i]);
// 			else printf("  %d  [  %d  ]\n", i, FAT[i]);
// 		}
// 		else {
// 			if(FAT[i] > 0 && FAT[i] < 10) printf(" %d  [   %d  ]\n", i, FAT[i]);
// 			else printf(" %d  [  %d  ]\n", i, FAT[i]);
// 		}
// 	}
// }

// void regravaFATnoDisco(int valor, int inicioBytes, int nbytes) {
// 	if (arquivo) {
// 		int i;

// 		fseek(arquivo, inicioBytes, SEEK_SET);

// 		for (i = 0; i < nbytes; i++)
// 			if (fwrite (&FAT[i], sizeof(int), 1, arquivo) != 1)
// 				printf("Não escreveu 1 int!\n");
// 	}
// }

int main(int argc, char* argv[]) {
	arquivo = fopen(argv[1], "r+b");

	imprimeInt(0, 5);		//superbloco
	imprimeChar(160, 160); 	//bitmap
	imprimeInt(320, 160); 	//FAT

	return 0;
}