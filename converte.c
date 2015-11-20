#include <stdio.h>
#include <stdlib.h>

#define TAM_BLOCO			  	 160

typedef struct arquivo {
	char* nome;
	int byteInicio;
	int tempoCriado;
	int tempoModificado;
	int tempoAcessado;
	int tamanho;
} Arquivo;

static FILE* arquivo = NULL;

int FAT[1000];

int leInt(FILE* bin, int inicioBytes) {
	if (bin) {
		int val;

		fseek(bin, inicioBytes, SEEK_SET);

		if (fread(&val, sizeof(int), 1, bin) == 1) {
			return val;
		}
	}

	return 0;
}

Arquivo leStruct(FILE* bin, int inicioBytes) {
	Arquivo val;
	
	if (bin) {
		fseek(bin, inicioBytes, SEEK_SET);
		
		if (fread(&val, sizeof(Arquivo), 1, bin) == 1) {
			return val;
		}
	}
	
	return val;
}

void imprimeDiretorio(FILE* bin, int inicioBytes) {
	Arquivo saida = leStruct(bin, inicioBytes+4);
	int n = leInt(bin, inicioBytes);

	printf("#Entradas: %d|\n", n);
	printf("Nome: %s|\n", saida.nome);
	printf("Inicio byte: %d|\n", saida.byteInicio);
	printf("Tamanho: %d|x|\n\n", saida.tamanho);
}


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
	int i;

	arquivo = fopen(argv[1], "r+b");

	printf("\n\nSUPERBLOCO\n\n");
	imprimeInt(0, 6);		//superbloco

	printf("\n\nBITMAP\n\n");
	imprimeChar(160, 160); 	//bitmap

	printf("\n\nFAT\n\n");
	imprimeInt(320, 160); 	//FAT

	printf("\n\nRAIZ\n\n");
	imprimeInt(960, 1);		//Raiz
	imprimeChar(964, 600);		//Raiz
	
	printf("\n\n---------------------------------\n\n");

	

	return 0;
}

