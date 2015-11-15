#include <stdio.h>
#include <stdlib.h>
#include "fs.h"

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


#define TAM_FS				     1600		
#define TAM_BLOCO			  	 16		

typedef struct atributos {
	int tempoCriado;
	int tempoModificado;
	int tempoAcessado;
	char nome[20];
	int tamNome;
} Atributos; 

typedef struct arquivo {
	int inicioFat;
	Atributos listaAtributos;
	Arquivo filho[10];
} Arquivo;

static FILE* arquivoMount = NULL;

static int criaFS(char* fname); 
static int existeFS(char* fname);
static void criaSuperBloco();
static void criaBitMap();
static void criaFAT();
static void criaRaiz();

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

// void imprimeFS() {
// 	if (arquivoMount) {
// 		char val;

// 		while (!feof(arquivoMount)) {
// 			if (fread(&val, sizeof(char), 1, arquivoMount) == 1) {
// 				printf("%d", val);
// 			}
// 		}
// 	}
// }

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

static void criaSuperBloco() {
	escreveInt(TAM_FS/TAM_BLOCO, 0, 4);			// escreve numero de blocos
	escreveInt(TAM_BLOCO, 4, 4);				// escreve inicio bitmap
	escreveInt(2*TAM_BLOCO, 8, 4);				// escreve inicio FAT
	escreveInt(3*TAM_BLOCO, 12, 4);				// escreve inicio Raiz
}

static void criaBitMap() {
	escreveChar(0, TAM_BLOCO, TAM_BLOCO);					// inicializa mapa de bits com 0
}

static void criaFAT() {
	escreveInt(-2, 2*TAM_BLOCO, 4*TAM_BLOCO);				// inicializa o FAT com -2
}

static void criaRaiz() {
	Arquivo entRaiz[5];



	escreveInt(-2, 3*TAM_BLOCO, 48);						// inicializa o FAT com -2
}