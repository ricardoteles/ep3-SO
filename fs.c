#include <stdlib.h>
#include "fs.h"
#include "dados.h"
#include "binario.h"

static void criaFS(char* fname); 
static int existeFS(char* fname);
static void carregaFS();

static int qtadeBlocos;
static int iniSuperbloco;
static int iniBitmap;
static int iniFat;
static int iniRaiz;
//static int iniDados;
static int changeFAT;

int umountFS() {
	if (arquivo != NULL) {
		if (changeFAT)
			regravaFATnoDisco(iniFat, iniRaiz-iniFat);
		
		fclose(arquivo);
		arquivo = NULL;
		return 1;
	}
	return 0;
}

int mountFS(char* fname) {
	// se o arquivo não foi aberto ainda
	if (arquivo == NULL) {
		// se o arquivo não foi criado ainda
		if (!existeFS(fname)) {	
			criaFS(fname);
		}

		arquivo = fopen(fname, "r+b");
		if (arquivo) {
			carregaFS();
			changeFAT = 0;
			return 1;
		}
	}

	return 0;
}

static void carregaFS() {
	iniSuperbloco = 0;
	qtadeBlocos = leInt(arquivo, iniSuperbloco);			// le numero de blocos do superbloco
	iniBitmap   = leInt(arquivo, iniSuperbloco + 4); 		// le inicio bitmap do superbloco
	iniFat      = leInt(arquivo, iniSuperbloco + 8);		// le inicio FAT do superbloco
	iniRaiz     = leInt(arquivo, iniSuperbloco + 12);		// le inicio Raiz do superbloco
	//iniDados    = leInt(arquivo, iniSuperbloco + 16);		// le inicio Dados do superbloco

	carregaFATnaMemoria(iniFat, TAM_FAT);

	printf("Carreguei os dados:\n");
	printf("%d %d %d %d\n", qtadeBlocos, iniBitmap, iniFat, iniRaiz);
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

static void criaFS(char* fname) {
	arquivo = fopen(fname, "wb");

	// ve se criou
	if (arquivo) {
		criaSuperBloco(arquivo);
		criaBitMap(arquivo);
		criaFAT(arquivo);
		criaRaiz();

		fclose(arquivo);
	}
}
