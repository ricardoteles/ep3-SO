#include "fs.h"
#include "superbloco.h"
#include "bitmap.h"
#include "fat.h"
#include "dados.h"

// #define TAM_FS				262144		/* tamanho do FS em bytes (= 256KB) */
// #define TAM_BLOCO			  4096		/* tamanho do bloco em bytes (= 4KB) */

// #define INICIO_SUPERBLOCO 		0
// #define TAM_SUPERBLOCO 			TAM_BLOCO
// #define INICIO_BITMAP 			TAM_BLOCO
// #define TAM_BITMAP 				TAM_BLOCO
// #define INICIO_FAT				2*TAM_BLOCO
// #define TAM_FAT					TAM_BLOCO
// #define INICIO_DADOS			3*TAM_BLOCO
// #define TAM_DADOS				61*TAM_BLOCO

static int criaFS(char* fname); 
static int existeFS(char* fname);

void escreveInt(int valor, int inicioBytes, int nbytes) {
	if (arquivoMount) {
		int i;
		int myVal = valor;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&myVal, sizeof(int), 1, arquivoMount) != 1)
				printf("Não escreveu 1 int!\n");
	}
}

void escreveChar(char valor, int inicioBytes, int nbytes) {
	if (arquivoMount) {
		int i;
		char myVal = valor;

		fseek(arquivoMount, inicioBytes, SEEK_SET);

		for (i = 0; i < nbytes; i++)
			if (fwrite (&myVal, sizeof(char), 1, arquivoMount) != 1)
				printf("Não escreveu 1 byte!\n");
	}
}

int mountFS(char* fname) {
	// se o arquivoMount não foi aberto ainda
	if (!arquivoMount) {
		// se o arquivoMount não foi criado ainda
		if (!existeFS(fname) && !criaFS(fname)) { // criar pode dar errado se caminho for incoerente	
			return 0;
		}

		arquivoMount = fopen(fname, "r+b");
		if (arquivoMount) {
			printf("\n");
			return 1;
		}
	}

	return 0;
}

static int existeFS(char* fname) {
	// r+b eh modo que obriga existencia do arquivoMount binario
	arquivoMount = fopen(fname, "r+b");

	if (!arquivoMount) { // se NULL é porque não existe
		return 0;
	}

	fclose(arquivoMount);
	return 1;
}

static int criaFS(char* fname) {
	arquivoMount = fopen(fname, "wb");

	// ve se criou
	if (arquivoMount) {
		criaSuperBloco();
		criaBitMap();
		criaFAT();
		// criaRaiz();

		fclose(arquivoMount);

		return 1;
	}
	return 0;
}